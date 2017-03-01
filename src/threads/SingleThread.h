//
// Created by igrek on 04/02/17.
//

#ifndef SINGLETHREAD_H
#define SINGLETHREAD_H

#include <string>

using namespace std;

class SingleThread {
public:
	SingleThread();

	virtual ~SingleThread();

	bool busy();

	void start();

protected:
	virtual void run() = 0;

	string threadName();

private:
	void runContainer();

	void* boostThread; // type void* to avoid including boost/thread.hpp in every file including header

	volatile bool started;
	/// if run() has completed
	volatile bool closed;
};


#endif //SINGLETHREAD_H
