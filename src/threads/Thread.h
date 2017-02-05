#ifndef THREAD_H
#define THREAD_H

#include <string>

using namespace std;

class Thread {
public:
    Thread();

    virtual ~Thread();

    static void sleepMs(int ms);

protected:
    virtual void run() = 0;

    /// must be set to true to complete init
    volatile bool init;

    volatile bool closeSignal;

    void* boostThread; // type void* to avoid including boost/thread.hpp in every including header

    string threadName();

private:
    void start();

};

#endif
