//
// Created by igrek on 04/02/17.
//

#include "Synchronizer.h"
#include "../config/ConfigLoader.h"
#include "../dispatcher/EventDispatcher.h"
#include "../events/DiffScanButtonClicked.h"
#include "../events/DiffRemovedButtonClicked.h"
#include "../events/ShowUIMessageRequest.h"
#include "../events/DiffListUpdateRequest.h"

Synchronizer::Synchronizer() {
	loadConfig();
	registerEvents();
	diffscanner = new DiffScanner();
}

Synchronizer::~Synchronizer() {
	for (Database* db : *databases) {
		delete db;
	}
	databases->clear();
}

void Synchronizer::registerEvents() {
	EventDispatcher::registerEventObserver<DiffScanButtonClicked>(this);
	EventDispatcher::registerEventObserver<DiffRemovedButtonClicked>(this);
}

void Synchronizer::onEvent(Event* e) {
	if (e->instanceof<DiffScanButtonClicked*>()) {
		scanDiffs();
	} else if (e->instanceof<DiffRemovedButtonClicked*>()) {
		int selectedIndex = e->cast<DiffRemovedButtonClicked*>()->selectedIndex;
		removeDiff(selectedIndex);
	}
}

void Synchronizer::scanDiffs() {
	try {
		Logger::info("scanning diffs");

		diffscanner->scanDiffs(databases);
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

void Synchronizer::removeDiff(int index) {
	if (index == -1 || index >= diffscanner->getDiffs()->size()) {
		EventDispatcher::sendNow(new ShowUIMessageRequest("no difference selected"));
	} else {
		diffscanner->getDiffs()->erase(diffscanner->getDiffs()->begin() + index);
		EventDispatcher::sendNow(new DiffListUpdateRequest(diffscanner->getDiffs()));
		EventDispatcher::sendNow(new ShowUIMessageRequest("difference removed"));
	}
}