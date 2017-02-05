#include "LoopThread.h"
#include "../logger/Logger.h"
#include <boost/thread.hpp>

LoopThread::LoopThread(int wait_for_close) {
    this->wait_for_close = wait_for_close;
    closed = false;
}

LoopThread::~LoopThread() {
    closeSignal = true;
    Logger::debug("Waiting for finishing thread "+threadName()+"...");
    for (int i = 0; i < wait_for_close && !closed; i++) {
        sleepMs(1);
    }
    if (!closed) {
        Logger::warn("time for finishing thread has exceeded - force interrupting.");
        boost::thread* boost_thread2 = reinterpret_cast<boost::thread*>(boostThread);
        boost_thread2->interrupt();
        delete boost_thread2;
        boostThread = nullptr;
    }
    Logger::debug("Loop thread "+threadName()+" has finished");
}

void LoopThread::run() {
    while (!closeSignal) {
        runLoop();
    }
    closed = true;
}
