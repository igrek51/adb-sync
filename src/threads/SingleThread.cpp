//
// Created by igrek on 04/02/17.
//

#include "SingleThread.h"
#include "../logger/Logger.h"
#include <boost/thread.hpp>

SingleThread::SingleThread() {
	closed = false;
	boostThread = reinterpret_cast<void*>(new boost::thread(
			boost::bind(&SingleThread::start, this)));
	Logger::debug("Thread has been created");
}

SingleThread::~SingleThread() {
	boost::thread* boost_thread2 = reinterpret_cast<boost::thread*>(boostThread);
	boost_thread2->interrupt();
	boost_thread2->detach();
	delete boost_thread2;
	boostThread = nullptr;
	if (!closed) {
		Logger::debug("Thread " + threadName() + " has been interrupted.");
	}
	Logger::debug("Thread has been destroyed");
}

bool SingleThread::busy() {
	return !closed;
}

void SingleThread::start() {
	Logger::debug("Thread " + threadName() + " has started");
	run();
	closed = true;
	Logger::debug("Thread " + threadName() + " has finished");
}

string SingleThread::threadName() {
	return typeid(*this).name();
//    return boost::typeindex::type_id_with_cvr<decltype(*this)>().pretty_name();
}