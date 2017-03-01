//
// Created by igrek on 21/02/17.
//
#ifndef ADBSYNC_THREADSTEST_H
#define ADBSYNC_THREADSTEST_H

#include "TestApp.h"
#include "../threads/SingleThread.h"
#include "../threads/LoopThread.h"
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

class Thread3 : public SingleThread {
public:
	virtual void run() {
		Logger::info("running thread 3.1");
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		Logger::info("running thread 3.2");
	}
};

class Loop1 : public LoopThread {
public:
	Loop1() : LoopThread(10) {}

	virtual void runLoop() {
		Logger::info("running loop..");
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
};

class ThreadsTest : public TestApp {
public:
	virtual void runTest() override {

		// running thread and waiting for finishing
		Logger::info("Test 1:");
		SingleThread* thread3 = new Thread3();
		thread3->start();
		while (thread3->busy()) {
			continue;
		}
		delete thread3;

		std::this_thread::sleep_for(std::chrono::milliseconds(150));
		// interrupting thread
		Logger::info("Test 2:");
		thread3 = new Thread3();
		thread3->start();
		std::this_thread::sleep_for(std::chrono::milliseconds(5));
		delete thread3;

		std::this_thread::sleep_for(std::chrono::milliseconds(150));
		// multithreading
		Logger::info("Test 3:");
		SingleThread* thread1 = new Thread1();
		SingleThread* thread2 = new Thread2();
		thread1->start();
		thread2->start();

		while (thread1->busy() || thread2->busy()) {
//			Logger::info("waiting...");
		}
		Logger::info("threads finished");

		delete thread1;
		delete thread2;

		std::this_thread::sleep_for(std::chrono::milliseconds(150));
		// loop thread
		Logger::info("Test 4:");
		Loop1* loop1 = new Loop1();
		loop1->start();
		std::this_thread::sleep_for(std::chrono::milliseconds(1500));
		delete loop1;

		std::this_thread::sleep_for(std::chrono::milliseconds(150));
	}
};

#endif //ADBSYNC_THREADSTEST_H