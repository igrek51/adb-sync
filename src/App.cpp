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
    Logger::debug("destroying App...");
    delete gui;
    delete synchronizer;
    delete qapp;
}

void App::signalTraceHandler(int sig) {
    void* array[10];
    int size = backtrace(array, 10);
    Logger::error("Signal " + to_string(sig) + " caught, stack trace:");
    backtrace_symbols_fd(array, size, STDOUT_FILENO);
    exit(1);
}

int App::run() {
    Logger::debug("running App...");
    //catching signals and printing stack traces
    signal(SIGSEGV, App::signalTraceHandler); // segmentation fault
    signal(SIGINT, App::signalTraceHandler);
    signal(SIGFPE, App::signalTraceHandler);
    signal(SIGILL, App::signalTraceHandler);

    try {
        return qapp->exec();
    } catch (Error* e) {
        Logger::error("uncaught Error: " + e->getMessage());
        delete e;
    } catch (const std::bad_alloc&) {
        Logger::fatal("std::bad_alloc error caught from QApplication.exec()");
    } catch (std::exception& e) {
        Logger::error("uncaught std::exception: " + string(e.what()));
    } catch (...) {
        Logger::error("unknown uncaught throwable");
    }
    return -1;
}



