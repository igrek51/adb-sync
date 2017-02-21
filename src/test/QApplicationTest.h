//
// Created by igrek on 21/02/17.
//

#ifndef ADBSYNC_QAPPLICATIONTEST_H
#define ADBSYNC_QAPPLICATIONTEST_H

#include "TestApp.h"
#include <QLabel>
#include <QMainWindow>
#include <QApplication>

class QApplicationTest : public TestApp {
public:
	virtual void runTest() override {

		int argc = 0;
		char** argv = new char* [0];
		QApplication application(argc, nullptr);
		QMainWindow mainWindow;
		QLabel* label = new QLabel(&mainWindow);
		label->setText("first line\nsecond line");
		mainWindow.show();
		Logger::info("returned " + to_string(application.exec()));

	}
};

#endif //ADBSYNC_QAPPLICATIONTEST_H
