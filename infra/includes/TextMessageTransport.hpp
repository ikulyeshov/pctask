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
#include "Thread.hpp"

namespace infra
{
namespace msgserver
{

class Transport: public Thread::User
{
public:
	struct Observer
	{
		virtual void OnMessage(int sockfd, const char* message) = 0;
	};

public:
	Transport(int sockfd, Observer* observer);
	~Transport();

	Status Listen();
	Status Write(const char* message);

	virtual void OnThread();

private:
	//not copyable
	Transport(const Transport& rop);
	const Transport operator=(const Transport& rop);

	char mBuffer[TEXT_MESSAGE_MAX_MESSAGE_LEN];
	char* mBufferPtr;
	Thread mThread;
	int mSocket;
	Observer* mObserver;

};

} //namespace msgserver
} //namespace infra


#endif /* INCLUDES_TEXTMESSAGETRANSPORT_HPP_ */
