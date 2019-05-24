
#ifndef MTHREAD_H_
#define MTHREAD_H_

#include <iostream>
#include <stdio.h>
#include <pthread.h>

using namespace std;

class MThread{
public:
	pthread_t threadId;
public:
	void start();
	void waitForThread();
	virtual ~MThread();
	virtual void run() = 0;
};


#endif /* MTHREAD_H_ */
