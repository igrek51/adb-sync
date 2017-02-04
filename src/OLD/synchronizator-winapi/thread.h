#ifndef THREAD_H
#define THREAD_H

#include <windows.h>

DWORD WINAPI thread_start_routine(void *args);

class Thread {
public:
    Thread();
    virtual ~Thread();
    virtual void execute() = 0;
    volatile bool init; //zakoñczono inicjalizacjê
    volatile bool close; //polecenie zamykania
protected:
    HANDLE handle;
};

class SingleThread : public Thread {
public:
    virtual ~SingleThread();
    virtual void run() = 0;
private:
    void execute();
};

class ContinuousThread : public Thread {
public:
    ContinuousThread(int wait_for_close = 0);
    virtual ~ContinuousThread();
    virtual void run() = 0;
private:
    void execute();
    int wait_for_close;
    volatile bool closed; //zakoñczono wykonywanie funkcji run()
};


class FileSearch : public SingleThread {
public:
    FileSearch();
    ~FileSearch();
    static volatile bool active;
private:
    void run();
};

#endif
