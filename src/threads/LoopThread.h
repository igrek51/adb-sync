#ifndef LOOP_THREAD_H
#define LOOP_THREAD_H

#include "Thread.h"

class LoopThread : public Thread {
public:
    LoopThread(int wait_for_close = 0);

    virtual ~LoopThread();

protected:

    virtual void run();

    virtual void runLoop() = 0;

private:

    int wait_for_close;
    /// if run() has completed
    volatile bool closed;
};

#endif
