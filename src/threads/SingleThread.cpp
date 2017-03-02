//
// Created by igrek on 04/02/17.
//

#include "SingleThread.h"
#include "../logger/Logger.h"
#include <boost/thread.hpp>

SingleThread::SingleThread() {
	started = false;
	closed = false;
	boostThread = nullptr;
}

SingleThread::~SingleThread() {
	if (boostThread != nullptr) {
		boost::thread* boost_thread2 = reinterpret_cast<boost::thread*>(boostThread);
		boost_thread2->interrupt();
		boost_thread2->detach();
		delete boost_thread2;
		boostThread = nullptr;
		if (!closed) {
			Logger::debug("Thread " + threadName() + " has been interrupted.");
		}
		closed = true;
	}
	Logger::debug("Thread has been destroyed");
}

bool SingleThread::busy() {
	return started && !closed;
}

void SingleThread::start() {
	if (started) {
		Logger::warn("Thread has been started already");
	}
	started = true;

	boostThread = reinterpret_cast<void*>(new boost::thread(
			boost::bind(&SingleThread::runContainer, this)));
}

string SingleThread::threadName() {
	return typeid(*this).name();
//    return boost::typeindex::type_id_with_cvr<decltype(*this)>().pretty_name();
}

void SingleThread::runContainer() {
	Logger::debug("Thread " + threadName() + " has been started");

	try {
		run();
	} catch (Error* e) {
		Logger::error("Uncaught error:" + e->getMessage());
		delete e;
	}

	if (closed) // if had been interrupted
		return;
	closed = true;
	Logger::debug("Thread " + threadName() + " has finished");
}
