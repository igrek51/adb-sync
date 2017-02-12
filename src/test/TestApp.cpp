//
// Created by igrek on 13/02/17.
//

#include "TestApp.h"
#include "../logger/Logger.h"
#include "../synchronizer/DiffScanner.h"

TestApp::TestApp(int argc, char** argv) : App(argc, argv) {}

int TestApp::run() {

    Logger::info("test start");

    try {

        LocalFS* localFS = new LocalFS();
        vector<File*>* files = localFS->listPath("../test/");
        for (File* file : *files) {
            Logger::info("file: " + file->getPathName());
        }

    } catch (Error* e) {
        Logger::error(e);
    }

    Logger::info("test end");

    return 0;
}