#include <iostream>
#include "logger/Logger.h"
#include "threads/SingleThread.h"
#include "threads/Thread.h"

using namespace std;

//TODO smart pointers, shared ...
//TODO c++ 11 standards: nullptr, foreach, auto, enum class


class TestThread1 : public SingleThread {

protected:
    virtual void run(){
        Logger::debug("thread " + threadName() + " start");
        Thread::sleepMs(1);
        Logger::debug("thread " + threadName() + " stop");
    }
};



int main(int argc, char** argv) {
//    App* app = new App(argc, argv);
//    delete app;

    TestThread1* thread1 = new TestThread1();

    while(thread1->busy())
        Logger::info("busy");

    Thread::sleepMs(50);
    delete thread1;
    Thread::sleepMs(200);
//    Thread::sleepMs(100);

    Logger::info("goodbye, world");

    return 0;
}


