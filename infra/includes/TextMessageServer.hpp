/*
 * TextMessageServer.hpp
 *
 *  Created on: Apr 15, 2016
 *      Author: ihor
 */

#ifndef INCLUDES_TEXTMESSAGESERVER_HPP_
#define INCLUDES_TEXTMESSAGESERVER_HPP_

#include <pthread.h>
#include <vector>
#include "TextMessageDefines.hpp"
#include "Types.hpp"
#include "TextMessageTransport.hpp"
#include "Mutex.hpp"
#include "Thread.hpp"

namespace infra
{
namespace msgserver
{

class Server: public Thread::User, public Transport::Observer
{

static const int MAX_ADDR = 50;

public:

	struct Observer
	{
		virtual void OnMessage(Handle connect, const char* message) = 0;
	};

	struct ClientDesc
	{
		int mPort;
		char mAddr[MAX_ADDR];
		int mSocket;
		infra::msgserver::Transport *mTransport;
	};

	typedef std::vector<ClientDesc> ClientsCollection;

public:
	Server(Observer* observer);
	~Server();

	Status Start(int port);
	Status Message(Handle client, const char* message);
	Status List();//List of the client connected

	virtual void OnMessage(int sockfd, const char* message);

private:
	//not copyable
	Server(const Server& rop);
	const Server operator=(const Server& rop);

	virtual void OnThread();

	int mSocket;
	sockaddr_in mServerAddr;
	sockaddr_in mClientAddr;
	Thread mThread;
	Mutex mLockMutex;
	ClientsCollection mClients;
	Observer* mObserver;

};

} //namespace msgserver
} //namespace infra


#endif /* INCLUDES_TEXTMESSAGESERVER_HPP_ */
