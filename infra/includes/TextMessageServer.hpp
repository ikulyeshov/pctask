/*
 * TextMessageServer.hpp
 *
 *  Created on: Apr 15, 2016
 *      Author: ihor
 */

#ifndef INCLUDES_TEXTMESSAGESERVER_HPP_
#define INCLUDES_TEXTMESSAGESERVER_HPP_


#include "TextMessageDefines.hpp"

namespace infra
{
namespace msgserver
{

class Server
{
public:
	Server();
	~Server();

	Status Start(int port);

private:
	//not copyable
	Server(const Server& rop);
	const Server operator=(const Server& rop);

};

} //namespace msgserver
} //namespace infra


#endif /* INCLUDES_TEXTMESSAGESERVER_HPP_ */
