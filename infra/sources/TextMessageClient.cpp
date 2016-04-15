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

Client::Client()
{
}

Client::~Client()
{

}

Status Client::Start(const char* addr, int port)
{
    int sockfd = -1, portno, n;
    struct sockaddr_in serv_addr;

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

		bzero((char *) &serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
		serv_addr.sin_port = htons(port);
		if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
		{
			ps_log_error("Error connecting");
			st = ST_CONNECT_ERROR;
			break;
		}

		Transport transport(sockfd, 0);
//		transport.Listen();

		char buffer[TEXT_MESSAGE_MAX_MESSAGE_LEN];

		do
		{
			printf("Please enter the message: ");
			bzero(buffer, 256);
			fgets(buffer, 255, stdin);


//			int n = write(sockfd, buffer, strlen(buffer));
			if ((st = transport.Write(buffer)) != ST_OK)
			{
				break;
			}
		}
		while (st == ST_OK && buffer[0] != 'c');

		if (st != ST_OK)
			break;
    }
    while (0);

    if (sockfd >= 0)
    {
    	close(sockfd);
    }

	return st;
}



