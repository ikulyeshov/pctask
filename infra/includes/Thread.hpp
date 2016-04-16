/*
 * Thread.hpp
 *
 *  Created on: Apr 16, 2016
 *      Author: ihor
 */

#ifndef INCLUDES_THREAD_HPP_
#define INCLUDES_THREAD_HPP_

#include <pthread.h>
#include "Types.hpp"

namespace infra
{

class Thread
{
public:
	struct User
	{
		virtual void OnThread() = 0;
	};

public:
	Thread(User* user);
	virtual ~Thread();

	Status Start();
	Status Stop();

private:
	//not copyable
	Thread(const Thread& rop);
	const Thread& operator=(const Thread& rop);

	static void* ThreadCallback(void *arg);
	void DoThread();

	User* mUser;
	pthread_t mThreadId;
	volatile int mStarted;
	volatile int mStop;

};

} //namespace infra



#endif /* INCLUDES_THREAD_HPP_ */
