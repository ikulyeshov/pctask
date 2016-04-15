/*
 * TextMessageServer.cpp
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

#include "Logger.hpp"
#include "TextMessageServer.hpp"
#include "TextMessageTransport.hpp"

using namespace infra::msgserver;

Server::Server()
{
}

Server::~Server()
{

}

Status Server::Start(int port)
{
	Status st = ST_OK;

	int sockfd = -1, newsockfd = -1, portno;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    do
    {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
		if (sockfd < 0)
		{
			ps_log_error("Error opening socket");
			st = ST_OPEN_ERROR;
			break;
		}

		bzero((char *) &serv_addr, sizeof(serv_addr));
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = INADDR_ANY;
		serv_addr.sin_port = htons(port);

		if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
		{
			ps_log_error("Error on binding");
			st = ST_BINDING_ERROR;
			break;
		}

		do
		{
			listen(sockfd,5);
			clilen = sizeof(cli_addr);
			newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

			if (newsockfd < 0)
			{
				ps_log_error("Error on accept");
				st == ST_ACCEPT_ERROR;
				break;
			}

			ps_log_info("Accept connection %i", newsockfd);

			infra::msgserver::Transport* transport = new infra::msgserver::Transport(newsockfd, 0);
			transport->Listen();
		}
		while (1);

    }
    while (0);

    if (sockfd >= 0)
    	close(sockfd);

	return st;
}



