#include "LoopThread.h"
#include "../logger/Logger.h"
#include <boost/thread.hpp>

LoopThread::LoopThread(int waitForCloseMs) {
	this->waitForCloseMs = waitForCloseMs;
	closeRequest = false;
}

LoopThread::~LoopThread() {
	closeRequest = true;
	Logger::debug("Waiting for finishing the thread " + threadName() + "...");
	for (int i = 0; i < waitForCloseMs && !closed; i++) {
		sleepMs(1);
	}
	if (!closed) { // if the thread still has not been closed
		Logger::warn("Time for finishing thread has exceeded - force interrupting...");
	}
}

void LoopThread::run() {
	while (!closeRequest) {
		runLoop();
	}
}

void LoopThread::sleepMs(int ms) {
	usleep((__useconds_t) (1000 * ms));
}
