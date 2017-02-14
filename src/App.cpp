//
// Created by igrek on 04/02/17.
//

#include "App.h"
#include "logger/Logger.h"

App::App(int argc, char** argv) {
    Logger::debug("constructing app");
    this->argc = argc;
    this->argv = argv;
    qapp = new QApplication(argc, argv);
    synchronizer = new Synchronizer();
    gui = new GUI();
}

App::~App() {
    Logger::debug("destructing app");
    delete gui;
    delete synchronizer;
    delete qapp;
}

int App::run() {

    Logger::info("hello");

//    try {
//
////        ADB* adb = new ADB();
////        vector<File*>* files = adb->listPath("/storage/extSdCard/guitarDB/0thers");
////        for (File* file : *files) {
////            Logger::info("file: " + file->getPathName());
////        }
//
//        DiffScanner* diffscanner = new DiffScanner();
//
//        diffscanner->scanDiffs();
//        vector<Diff*>* diffs = diffscanner->getDiffs();
//
//        for (Diff* diff : *diffs) {
//            Logger::info("diff: " + diff->localFile + ", type: " + to_string((int) diff->type));
//        }
//
//    } catch (Error* e) {
//        Logger::error(e);
//    }

    try {
        return qapp->exec();
    } catch (const std::bad_alloc&) {
        Logger::fatal("std::bad_alloc error caught from QApplication.exec()");
        return -1;
    }
}



