#ifndef LOOP_THREAD_H
#define LOOP_THREAD_H

#include "SingleThread.h"

class LoopThread : public SingleThread {
public:
	/**
	 * @param waitForCloseMs milliseconds waiting for self closing the thread before force close
	 */
	LoopThread(int waitForCloseMs = 0);

	virtual ~LoopThread();

protected:
	virtual void run() override;

	virtual void runLoop() = 0;

	static void sleepMs(int ms);

	volatile bool closeRequest;

private:
	int waitForCloseMs;
};

#endif
