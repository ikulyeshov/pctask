/*
 * Thread.cpp
 *
 *  Created on: Apr 16, 2016
 *      Author: ihor
 */

#include <unistd.h>
#include "Thread.hpp"

Thread::Thread(User* user):
		mUser(user),
		mThreadId(-1),
		mStarted(0),
		mStop(0)
{

}

Thread::~Thread()
{

}

Status Thread::Start()
{
	Status st = ST_OK;

	if (mStarted)
		return st;

	mStop = 0;

	st = pthread_create(&mThreadId, 0, &ThreadCallback, this) ? ST_ERROR : ST_OK;

	if (st == ST_OK)
	{
		mStarted = 1;
	}

	return st;
}

Status Thread::Stop()
{
	Status st = ST_OK;

	if (!mStarted)
		return st;

	mStop = 1;

	//implement more graceful shutdown
	while (mStarted)
	{
		usleep(10000);
	}

	mThreadId = -1;

	return st;
}

void* Thread::ThreadCallback(void *arg)
{
	static_cast<Thread*>(arg)->DoThread();
}

void Thread::DoThread()
{
	do
	{
		if (mUser)
		{
			mUser->OnThread();
		}
	}
	while (!mStop);

	mStarted = 0;
	mStop = 0;
}


