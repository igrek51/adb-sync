//
// Created by igrek on 04/02/17.
//

#include "App.h"
#include "logger/Logger.h"

App::App(int argc, char** argv) {
    this->argc = argc;
    this->argv = argv;
    a = new QApplication(argc, argv);
    gui = new GUI();
    synchronizer = new Synchronizer();
}

App::~App() {
    delete gui;
    delete synchronizer;
    delete a;
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

    return a->exec();
}



