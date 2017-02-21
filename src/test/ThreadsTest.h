//
// Created by igrek on 21/02/17.
//
#ifndef ADBSYNC_THREADSTEST_H
#define ADBSYNC_THREADSTEST_H

#include "TestApp.h"
#include "../threads/SingleThread.h"


class Thread1 : public SingleThread {
public:
	virtual void run() {
		Logger::info("running thread 1");
		Logger::info("running thread 1");
		Logger::info("running thread 1");
		Logger::info("running thread 1");
		Logger::info("running thread 1");
		Logger::info("running thread 1");
	}
};

class Thread2 : public SingleThread {
public:
	virtual void run() {
		Logger::info("running thread 2");
		Logger::info("running thread 2");
		Logger::info("running thread 2");
		Logger::info("running thread 2");
		Logger::info("running thread 2");
		Logger::info("running thread 2");
	}
};

class ThreadsTest : public TestApp {
public:
	virtual void runTest() {

		SingleThread* thread1 = new Thread1();
		SingleThread* thread2 = new Thread2();

		while (thread1->busy() || thread2->busy()) {
			Logger::info("waiting...");
		}
		Logger::info("threads finished");

		delete thread1;
		delete thread2;
	}
};

#endif //ADBSYNC_THREADSTEST_H