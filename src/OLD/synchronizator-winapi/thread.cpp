#include "thread.h"
#include "app.h"

DWORD WINAPI thread_start_routine(void *args){
    Thread *thread = (Thread*)args;
    while(!thread->init) Sleep(1);
    IO::geti()->log("Wykonywanie w�tku...");
    thread->execute();
    IO::geti()->log("Zako�czono wykonywanie w�tku.");
    //ExitThread(0);
    if(!thread->close) delete thread;
    return 0;
}

Thread::Thread(){
    init = false;
    close = false;
    DWORD watek_id;
    handle = CreateThread(NULL, 0, thread_start_routine, (void *)this, 0, &watek_id);
    IO::geti()->log("W�tek zainicjowany.");
}

Thread::~Thread(){
    IO::geti()->log("W�tek zako�czony.");
}


SingleThread::~SingleThread(){
    close = true;
    TerminateThread(handle, 0);
    CloseHandle(handle);
    IO::geti()->log("Pojedynczy w�tek zako�czony.");
}

void SingleThread::execute(){
    run();
}


ContinuousThread::ContinuousThread(int wait_for_close){
    this->wait_for_close = wait_for_close;
    closed = false;
}

ContinuousThread::~ContinuousThread(){
    close = true;
    IO::geti()->log("Oczekiwanie na zako�czenie w�tku.");
    for(int i=0; i<wait_for_close && !closed; i++){
        Sleep(1);
    }
    if(!closed){
        IO::geti()->log("Przekroczono czas oczekiwania - si�owe zamykanie w�tku.");
        TerminateThread(handle, 0);
        CloseHandle(handle);
    }
    IO::geti()->log("W�tek ci�g�y zako�czony.");
}

void ContinuousThread::execute(){
    while(!close){
        run();
    }
    closed = true;
}


volatile bool FileSearch::active = false;

FileSearch::FileSearch(){
    if(active){
        IO::geti()->error("W�tek przeszukiwania plik�w by� ju� uruchomiony.");
    }
    active = true;
    IO::geti()->log("W�tek przeszukiwania plik�w zainicjowany.");
    init = true;
}

FileSearch::~FileSearch(){
    active = false;
    IO::geti()->log("W�tek przeszukiwania plik�w zako�czony.");
}

void FileSearch::run(){
    App::geti()->synchro_search();
}

void App::filesearch_start(){
    if(FileSearch::active) return;
    filesearch = new FileSearch();
}
