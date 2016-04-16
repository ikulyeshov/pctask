/*
 * Mutex.hpp
 *
 *  Created on: Apr 16, 2016
 *      Author: ihor
 */

#ifndef INCLUDES_MUTEX_HPP_
#define INCLUDES_MUTEX_HPP_

#include <pthread.h>

namespace infra
{

class Mutex
{
public:
	Mutex()
	{
		pthread_mutex_init(&mMutex, 0);
	}
	virtual ~Mutex()
	{
		pthread_mutex_destroy(&mMutex);
	}

	void Lock()
	{
		pthread_mutex_lock(&mMutex);
	}

	void Unlock()
	{
		pthread_mutex_unlock(&mMutex);
	}

private:
	//not copyable
	Mutex(const Mutex& rop);
	const Mutex& operator=(const Mutex& rop);

	pthread_mutex_t mMutex;

};

class Locker
{
public:
	Locker(Mutex& mutex):
		mMutex(mutex)
	{
		mMutex.Lock();
	}
	virtual ~Locker()
	{
		mMutex.Unlock();
	}
private:
	//not copyable
	Locker(const Locker& rop);
	const Locker& operator=(const Locker& rop);

	Mutex &mMutex;
};

} //namespace infra


#endif /* INCLUDES_MUTEX_HPP_ */
