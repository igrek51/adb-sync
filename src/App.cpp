//
// Created by igrek on 04/02/17.
//

#include "App.h"
#include "logger/Logger.h"
#include "utils/string_utils.h"
#include "filesystem/ADB.h"
#include <QApplication>
#include <execinfo.h>
#include <unistd.h>

void backtraceHandler(int sig) {
    void* array[10];
    int size;
    size = backtrace(array, 10);
    Logger::error("Error signal " + itos(sig) + ":");
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}

App::App(int argc, char** argv) {
    // catching errors, printing stack trace
//    signal(SIGSEGV, backtraceHandler);

    this->argc = argc;
    this->argv = argv;
    gui = new GUI();
    synchronizer = new Synchronizer();
}

App::~App() {

}

int App::run() {

    Logger::info("hello");

    try {

        ADB* adb = new ADB();
        vector<File*>* files = adb->listPath("/storage/extSdCard/tmp");

        for (File* file : *files) {
            Logger::info("file: " + file->getName());
        }

    } catch (Error* e) {
        Logger::error(e);
    }


    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}



