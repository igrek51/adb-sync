//
// Created by igrek on 04/02/17.
//

#include "App.h"
#include "logger/Logger.h"

#include <execinfo.h>
#include <signal.h>
#include <unistd.h>

App::App(int argc, char** argv) {
    Logger::debug("constructing App...");
    this->argc = argc;
    this->argv = argv;
    qapp = new QApplication(argc, argv);
    synchronizer = new Synchronizer();
    gui = new GUI();
}

App::~App() {
    Logger::debug("destructing App...");
    delete gui;
    delete synchronizer;
    delete qapp;
}

void App::stackTraceHandler(int sig) {
    void* array[10];
    int size = backtrace(array, 10);

    fprintf(stderr, "Error: signal %d:\n", sig);
    Logger::error("Signal " + to_string(sig) + ", stack trace:");
    backtrace_symbols_fd(array, size, STDOUT_FILENO);
    exit(1);
}

int App::run() {

    Logger::info("running App...");
    //catching signals and printing stack trace
    signal(SIGSEGV, App::stackTraceHandler); // segmentation fault
    signal(SIGINT, App::stackTraceHandler);
    signal(SIGFPE, App::stackTraceHandler);
    signal(SIGILL, App::stackTraceHandler);

    try {
        return qapp->exec();
    } catch (const std::bad_alloc&) {
        Logger::fatal("std::bad_alloc error caught from QApplication.exec()");
        return -1;
    }
}



