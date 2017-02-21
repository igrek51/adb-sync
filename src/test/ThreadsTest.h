//
// Created by igrek on 21/02/17.
//
#ifndef ADBSYNC_THREADSTEST_H
#define ADBSYNC_THREADSTEST_H

#include "TestApp.h"
#include "../threads/SingleThread.h"
#include <thread>


class Thread1 : public SingleThread {
public:
	virtual void run() {
		Logger::info("running thread 1.1");
		Logger::info("running thread 1.2");
		Logger::info("running thread 1.3");
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
		Logger::info("running thread 1.4");
		Logger::info("running thread 1.5");
		Logger::info("running thread 1.6");
	}
};

class Thread2 : public SingleThread {
public:
	virtual void run() {
		Logger::info("running thread 2.1");
		Logger::info("running thread 2.2");
		Logger::info("running thread 2.3");
		Logger::info("running thread 2.4");
		Logger::info("running thread 2.5");
		Logger::info("running thread 2.6");
	}
};

class ThreadsTest : public TestApp {
public:
	virtual void runTest() override {

		SingleThread* thread1 = new Thread1();
		SingleThread* thread2 = new Thread2();

		while (thread1->busy() || thread2->busy()) {
//			Logger::info("waiting...");
		}
		Logger::info("threads finished");

		delete thread1;
		delete thread2;
	}
};

#endif //ADBSYNC_THREADSTEST_H