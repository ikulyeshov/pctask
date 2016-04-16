/*
 * TextMessageClient.cpp
 *
 *  Created on: Apr 15, 2016
 *      Author: ihor
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "Logger.hpp"
#include "TextMessageClient.hpp"
#include "TextMessageTransport.hpp"

using namespace infra::msgserver;

Client::Client(Observer* observer):
		mObserver(observer)
{
}

Client::~Client()
{

}

Status Client::Message(const char* message)
{
	Status st = ST_OK;
	mTransport->Write(message);
	return st;
}

Status Client::Start(const char* addr, int port)
{
    int sockfd = -1;

    Status st = ST_OK;

    do
    {
        struct hostent *server = gethostbyname(addr);

        sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0)
		{
			ps_log_error("Error opening socket");
			st = ST_OPEN_ERROR;
			break;
		}

		if (server == NULL)
		{
			ps_log_error("No such host\n");
			st = ST_PARAM_ERROR;
			break;
		}

		bzero((char *) &mServAddr, sizeof(mServAddr));
		mServAddr.sin_family = AF_INET;
		bcopy((char *)server->h_addr, (char *)&mServAddr.sin_addr.s_addr, server->h_length);
		mServAddr.sin_port = htons(port);
		if (connect(sockfd,(struct sockaddr *) &mServAddr,sizeof(mServAddr)) < 0)
		{
			ps_log_error("Error connecting");
			st = ST_CONNECT_ERROR;
			break;
		}

		mTransport = new Transport(sockfd, this);
		mTransport->Listen();
    }
    while (0);

    if (sockfd >= 0 && st != ST_OK)
    {
    	close(sockfd);
    }

	return st;
}

void Client::OnMessage(int sockfd, const char* message)
{
	if (mObserver)
		mObserver->OnMessage(message);
}


