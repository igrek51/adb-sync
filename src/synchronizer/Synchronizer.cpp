//
// Created by igrek on 04/02/17.
//

#include "Synchronizer.h"
#include "../config/ConfigLoader.h"

Synchronizer::Synchronizer() {
    // loading config
    ConfigLoader* loader = new ConfigLoader();
    databases = loader->loadDatabases();
    delete loader;
}

Synchronizer::~Synchronizer() {
    for(Database* db : *databases){
        delete db;
    }
    databases->clear();
}
