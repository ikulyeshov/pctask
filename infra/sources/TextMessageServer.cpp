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

using namespace infra::msgserver;

Server::Server():
		mSocket(-1),
		mThreadId(-1)
{
//	mClients.res(10);
	pthread_mutex_init(&mLockMutex, 0);
}

Server::~Server()
{
	pthread_mutex_destroy(&mLockMutex);
}

void Server::Lock()
{
	pthread_mutex_lock(&mLockMutex);
}

void Server::Unlock()
{
	pthread_mutex_unlock(&mLockMutex);
}

void* Server::ThreadCallback(void *arg)
{
	static_cast<Server*>(arg)->DoThread();
}

void Server::DoThread()
{
	do
	{
		listen(mSocket, 5);
		socklen_t clilen = sizeof(mClientAddr);
		int newsockfd = accept(mSocket, (struct sockaddr *) &mClientAddr, &clilen);

		if (newsockfd < 0)
		{
			ps_log_error("Error on accept");
			continue;
		}

		Lock();
		ps_log_info("Accept connection %i %i", mClients.size(), mClientAddr.sin_port);

		mClients.push_back(ClientDesc());
		mClients.back().mPort = mClientAddr.sin_port;
		infra::msgserver::Transport *transport = new infra::msgserver::Transport(newsockfd, 0);
		mClients.back().mTransport = transport;
		mClients.back().mTransport->Listen();
		Unlock();
	}
	while (1);
}

Status Server::Start(int port)
{
	Status st = ST_OK;

    socklen_t clilen;

    do
    {
        mSocket = socket(AF_INET, SOCK_STREAM, 0);
		if (mSocket < 0)
		{
			ps_log_error("Error opening socket");
			st = ST_OPEN_ERROR;
			break;
		}

		bzero((char *) &mServerAddr, sizeof(mServerAddr));
		mServerAddr.sin_family = AF_INET;
		mServerAddr.sin_addr.s_addr = INADDR_ANY;
		mServerAddr.sin_port = htons(port);

		if (bind(mSocket, (struct sockaddr *) &mServerAddr, sizeof(mServerAddr)) < 0)
		{
			ps_log_error("Error on binding");
			st = ST_BINDING_ERROR;
			break;
		}

		st = pthread_create(&mThreadId, 0, &ThreadCallback, this) ? ST_ERROR : ST_OK;

    }
    while (0);

    if (mSocket >= 0 && st != ST_OK)
    {
    	close(mSocket);
    	mSocket = -1;
    }

	return st;
}

Status Server::Message(Handle client, const char* message)
{
	Status st = ST_OK;
	Lock();
	if (client >= 0 && client < mClients.size())
	{
		ps_log_debug("Sending message %s", message);
		mClients[client].mTransport->Write(message);
		ps_log_debug("Finish sending");
	}
	Unlock();
	return st;
}

Status Server::List()
{
	Status st = ST_OK;

	Lock();
	ClientsCollection::iterator it = mClients.begin();
	while (it != mClients.end())
	{
		ps_log_info("Client %i: port %i", it - mClients.begin(), it->mPort);
		++it;
	}
	Unlock();

	return st;
}
