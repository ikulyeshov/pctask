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

namespace infra
{
namespace msgserver
{

class Server
{

static const int MAX_ADDR = 50;

public:

struct ClientDesc
{
	int mPort;
	char mAddr[MAX_ADDR];
	infra::msgserver::Transport *mTransport;
};

typedef std::vector<ClientDesc> ClientsCollection;

public:
	Server();
	~Server();

	Status Start(int port);
	Status Message(Handle client, const char* message);
	Status List();//List of the client connected

private:
	//not copyable
	Server(const Server& rop);
	const Server operator=(const Server& rop);

	static void* ThreadCallback(void *arg);
	void DoThread();

	void Lock();
	void Unlock();

	int mSocket;
	sockaddr_in mServerAddr;
	sockaddr_in mClientAddr;
	pthread_t mThreadId;
	pthread_mutex_t mLockMutex;
	ClientsCollection mClients;

};

} //namespace msgserver
} //namespace infra


#endif /* INCLUDES_TEXTMESSAGESERVER_HPP_ */
