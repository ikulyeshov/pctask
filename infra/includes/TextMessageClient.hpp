/*
 * TextMessageClient.hpp
 *
 *  Created on: Apr 15, 2016
 *      Author: ihor
 */

#ifndef INCLUDES_TEXTMESSAGECLIENT_HPP_
#define INCLUDES_TEXTMESSAGECLIENT_HPP_

#include "TextMessageDefines.hpp"
#include "Types.hpp"
#include "TextMessageTransport.hpp"

namespace infra
{
namespace msgserver
{

class Client: public Transport::Observer
{
public:
	struct Observer
	{
		virtual void OnMessage(const char* message) = 0;
	};

public:
	Client(Observer* observer);
	~Client();

	Status Start(const char* addr, int port);
	Status Message(const char* message);

	void OnMessage(int sockfd, const char* message);

private:
	//not copyable
	Client(const Client& rop);
	const Client operator=(const Client& rop);

    sockaddr_in mServAddr;
	Transport *mTransport;
	Observer* mObserver;

};

} //namespace msgserver
} //namespace infra

#endif /* INCLUDES_TEXTMESSAGECLIENT_HPP_ */
