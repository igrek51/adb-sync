//
// Created by igrek on 13/02/17.
//

#include "TestApp.h"
#include <execinfo.h>
#include <signal.h>
#include <unistd.h>

TestApp::TestApp() {}

void TestApp::signalTraceHandler(int sig) {
	void* array[10];
	int size = backtrace(array, 10);
	Logger::error("Signal " + signalName(sig) + " caught, stack trace:");
	backtrace_symbols_fd(array, size, STDOUT_FILENO);
	exit(1);
}

int TestApp::run() {

	//catching signals and printing stack traces
	signal(SIGSEGV, TestApp::signalTraceHandler); // segmentation fault
	signal(SIGINT, TestApp::signalTraceHandler);
	signal(SIGILL, TestApp::signalTraceHandler);

	try {

		Logger::info("TEST started");
		runTest();
		Logger::info("TEST ended");
		return 0;

	} catch (Error* e) {
		Logger::error("uncaught Error: " + e->getMessage());
		delete e;
	} catch (const std::bad_alloc&) {
		Logger::fatal("std::bad_alloc error caught from QApplication.exec()");
	} catch (std::exception& e) {
		Logger::error("uncaught std::exception: " + string(e.what()));
	} catch (...) {
		Logger::error("unknown uncaught throwable");
	}
	return -1;
}

string TestApp::signalName(int number) {
	switch (number) {
		case SIGSEGV:
			return "Segmentation violation";
		case SIGINT:
			return "Interrupt";
		case SIGILL:
			return "Illegal instruction";
	}
	return to_string(number);
}
