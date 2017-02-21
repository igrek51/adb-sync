//
// Created by igrek on 21/02/17.
//

#ifndef ADBSYNC_LOCALFSTEST_H
#define ADBSYNC_LOCALFSTEST_H

#include "TestApp.h"

class LocalFSTest : public TestApp {
public:
	virtual void runTest() override {

		LocalFS* localFS = new LocalFS();
		vector<File*>* files = localFS->listPath("../test/");
		for (File* file : *files) {
			Logger::info("file: " + file->getFullPathName());
		}

	}
};

#endif //ADBSYNC_LOCALFSTEST_H



