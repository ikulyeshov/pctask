/*
 * TextMessageTransport.hpp
 *
 *  Created on: Apr 15, 2016
 *      Author: ihor
 */

#ifndef INCLUDES_TEXTMESSAGETRANSPORT_HPP_
#define INCLUDES_TEXTMESSAGETRANSPORT_HPP_

#include <pthread.h>

#include "TextMessageDefines.hpp"
#include "Types.hpp"

namespace infra
{
namespace msgserver
{

class Transport
{
public:
	struct Observer
	{
		Observer(){}
		~Observer(){}

		virtual void OnMessage(const char* message) = 0;
	};

public:
	Transport(int sockfd, Observer* observer);
	~Transport();

	Status Listen();
	Status Write(const char* message);

private:
	//not copyable
	Transport(const Transport& rop);
	const Transport operator=(const Transport& rop);

	static void* ThreadCallback(void *arg);

	void DoThread();

	char mBuffer[TEXT_MESSAGE_MAX_MESSAGE_LEN];
	char* mBufferPtr;
	pthread_t mThreadId;
	int mSocket;
	Observer* mObserver;

};

} //namespace msgserver
} //namespace infra


#endif /* INCLUDES_TEXTMESSAGETRANSPORT_HPP_ */
