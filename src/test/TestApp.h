//
// Created by igrek on 13/02/17.
//

#ifndef ADBSYNC_TESTAPP_H
#define ADBSYNC_TESTAPP_H

#include "../App.h"
#include "../logger/Logger.h"

class TestApp {
public:
	TestApp();

	virtual int run();

	virtual void runTest() = 0;

private:
	static void signalTraceHandler(int sig);
};


#endif //ADBSYNC_TESTAPP_H
