/*
 * TextMessageTransport.cpp
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
#include "TextMessageTransport.hpp"

using namespace infra::msgserver;

Transport::Transport(int sockfd, Observer* observer):
		mThreadId(0),
		mSocket(sockfd),
		mBufferPtr(0),
		mObserver(observer)
{
}

Transport::~Transport()
{
}

void* Transport::ThreadCallback(void *arg)
{
	static_cast<Transport*>(arg)->DoThread();
}

Status Transport::Listen()
{
	Status st = ST_OK;

	st = pthread_create(&mThreadId, 0, &ThreadCallback, this) ? ST_ERROR : ST_OK;

	return st;
}

Status Transport::Write(const char* message)
{
	Status st = ST_OK;

	int n = write(mSocket, message, strlen(message));
	if (n < 0)
	{
		ps_log_error("Error writing to socket");
		st = ST_WRITE_ERROR;
	}
//	write(mSocket, "\n", 1);

	return st;
}

void Transport::DoThread()
{
	bzero(mBuffer, TEXT_MESSAGE_MAX_MESSAGE_LEN);

	fd_set read_fd;
	FD_ZERO (&read_fd);
	FD_SET (mSocket, &read_fd);

	Status st = ST_OK;
	mBufferPtr = mBuffer;

	while (st == ST_OK)
	{
		if (select(FD_SETSIZE, &read_fd, NULL, NULL, NULL) < 0)
		{
			ps_log_error ("Error on select");
			st = ST_SELECT_ERROR;
			continue;
		}

		for (int i = 0; i < FD_SETSIZE && st == ST_OK; ++i)
		{
			if (FD_ISSET (i, &read_fd))
			{
				if (i == mSocket)
				{
					int n = read(mSocket, mBufferPtr, TEXT_MESSAGE_MAX_MESSAGE_LEN - (mBufferPtr - mBuffer) /*space left*/);
					mBufferPtr += n;

					char* prevToken = mBuffer;
					int j = 0;
					for (j = 0; j < mBufferPtr - mBuffer; ++j)
					{
						if (mBuffer[j] == '\n')
						{
							mBuffer[j] = '\0';
							ps_log_debug("%i Msg -> %s", mSocket, prevToken);
							if (mObserver)
							{
								mObserver->OnMessage(prevToken);
							}
							prevToken = mBuffer + j + 1;
						}
					}

					if (prevToken == mBuffer && j == TEXT_MESSAGE_MAX_MESSAGE_LEN)
					{
						ps_log_error("Two long message, reset");
						mBufferPtr = mBuffer;
					}

					if (prevToken < mBufferPtr) /*some unfinished data in. Put to the begin of buffer*/
					{
						if (prevToken != mBuffer)
						{
							int j = 0;
							for (j = 0; j < mBufferPtr - prevToken; ++j)
							{
								mBuffer[j] = prevToken[j];
							}
							mBufferPtr = mBuffer + j;
						}
					}
					else
					{
						mBufferPtr = mBuffer;
					}
				}
			}
		}
	}
}

