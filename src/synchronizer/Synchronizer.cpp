//
// Created by igrek on 04/02/17.
//

#include "Synchronizer.h"
#include "../config/ConfigLoader.h"
#include "../dispatcher/EventDispatcher.h"
#include "../events/DiffScanButtonClicked.h"
#include "DiffScanner.h"

Synchronizer::Synchronizer() {
    loadConfig();
    registerEvents();
}

Synchronizer::~Synchronizer() {
    for(Database* db : *databases){
        delete db;
    }
    databases->clear();
}

void Synchronizer::registerEvents() {
    EventDispatcher::registerEventObserver<DiffScanButtonClicked>(this);
}

void Synchronizer::onEvent(Event* e) {
    if (e->instanceof<DiffScanButtonClicked*>()) {
        scanDiffs();
    }
}

void Synchronizer::scanDiffs() {

    try {

        Logger::info("scanning diffs");
        DiffScanner* diffscanner = new DiffScanner();

        diffscanner->scanDiffs();
        vector<Diff*>* diffs = diffscanner->getDiffs();

        for (Diff* diff : *diffs) {
            Logger::info("diff: " + diff->localFile + ", type: " + to_string((int) diff->type));
        }

    } catch (Error* e) {
        Logger::error(e);
    }
}

void Synchronizer::loadConfig() {
    ConfigLoader* loader = new ConfigLoader();
    databases = loader->loadDatabases();
    delete loader;
}
