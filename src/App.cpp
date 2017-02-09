//
// Created by igrek on 04/02/17.
//

#include "App.h"
#include "logger/Logger.h"
#include "utils/string_utils.h"
#include "synchronizer/DiffScanner.h"
#include <QApplication>

App::App(int argc, char** argv) {
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

//        ADB* adb = new ADB();
//        vector<File*>* files = adb->listPath("/storage/extSdCard/guitarDB/0thers");
//        for (File* file : *files) {
//            Logger::info("file: " + file->getPathName());
//        }

        DiffScanner* diffscanner = new DiffScanner();

        diffscanner->scanDiffs();
        vector<Diff*>* diffs = diffscanner->getDiffs();

        for (Diff* diff : *diffs) {
            Logger::info("diff: " + diff->localFile + ", type: " + to_string((int) diff->type));
        }

    } catch (Error* e) {
        Logger::error(e);
    }


    QApplication a(argc, argv);
    //TODO start MainWindow in GUI
    MainWindow w;
    w.show();

    return a.exec();
}



