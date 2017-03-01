//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_APP_H
#define ADBSYNC_APP_H


#include "synchronizer/Synchronizer.h"
#include "gui/GUI.h"
#include <QApplication>

class App {
public:
	App(int argc, char** argv);

	virtual ~App();

	virtual int run();

protected:
	int argc;
	char** argv;

private:
	QApplication* qapp;

	GUI* gui;

	Synchronizer* synchronizer;

	static void signalTraceHandler(int sig);

	static string signalName(int number);
};


#endif //ADBSYNC_APP_H
