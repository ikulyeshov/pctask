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

class Client
{
public:
	Client();
	~Client();

	Status Start(const char* addr, int port);
	Status Message(const char* message);

private:
	//not copyable
	Client(const Client& rop);
	const Client operator=(const Client& rop);

	Transport *mTransport;

};

} //namespace msgserver
} //namespace infra

#endif /* INCLUDES_TEXTMESSAGECLIENT_HPP_ */
