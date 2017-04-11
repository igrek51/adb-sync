//
// Created by igrek on 21/02/17.
//

#ifndef ADBSYNC_BUSYBOXTEST_H
#define ADBSYNC_BUSYBOXTEST_H

#include "TestApp.h"

class BusyboxTest : public TestApp {
public:
	virtual void runTest() override {
		Logger::info("checking busybox");
		ADB* adb = new ADB();
		adb->checkBusyBox();
	}
};

#endif //ADBSYNC_BUSYBOXTEST_H



