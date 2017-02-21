#include "Thread.h"
#include "../logger/Logger.h"
#include <boost/thread.hpp>

Thread::Thread() {
	init = false;
	closeSignal = false;
	boostThread = reinterpret_cast<void*>(new boost::thread(boost::bind(&Thread::start, this)));
	Logger::debug("Thread has been created");
}

Thread::~Thread() {
	closeSignal = true;
	if (boostThread != nullptr) {
		boost::thread* boost_thread2 = reinterpret_cast<boost::thread*>(boostThread);
		boost_thread2->interrupt();
		boost_thread2->detach();
		delete boost_thread2;
		boostThread = nullptr;
		Logger::debug("Thread has been destroyed");
	}
}

void Thread::start() {
	while (!init) sleepMs(1);
	Logger::debug("Thread " + threadName() + " is running...");
	run();
	if (closeSignal) {
		Logger::debug("Thread " + threadName() + " has been stopped remotely.");
	} else {
		Logger::debug("Thread " + threadName() + " has finished.");
	}
}

void Thread::sleepMs(int ms) {
	usleep((__useconds_t) (1000 * ms));
}

string Thread::threadName() {
	return typeid(*this).name();
//    return boost::typeindex::type_id_with_cvr<decltype(*this)>().pretty_name();
}