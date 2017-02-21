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
#include "../events/ExecuteDiffButtonClicked.h"
#include "../events/ExecuteAllDiffsButtonClicked.h"
#include "DiffSync.h"

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
	EventDispatcher::registerEventObserver<ExecuteDiffButtonClicked>(this);
	EventDispatcher::registerEventObserver<ExecuteAllDiffsButtonClicked>(this);
}

void Synchronizer::onEvent(Event* e) {
	if (e->instanceof<DiffScanButtonClicked*>()) {
		scanDiffs();
	} else if (e->instanceof<DiffRemovedButtonClicked*>()) {
		int selectedIndex = e->cast<DiffRemovedButtonClicked*>()->selectedIndex;
		removeDiff(selectedIndex);
	} else if (e->instanceof<ExecuteDiffButtonClicked*>()) {
		int selectedIndex = e->cast<ExecuteDiffButtonClicked*>()->index;
		syncDiff(selectedIndex);
	} else if (e->instanceof<ExecuteAllDiffsButtonClicked*>()) {
		syncAllDiffs();
	}
}

void Synchronizer::scanDiffs() {
	try {
		//TODO execute in another thread

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
	if (index == -1 || index >= (int) diffscanner->getDiffs()->size()) {
		EventDispatcher::sendNow(new ShowUIMessageRequest("no difference selected"));
	} else {
		Diff* diff = diffscanner->getDiffs()->at((unsigned long) index);
		delete diff;
		diffscanner->getDiffs()->erase(diffscanner->getDiffs()->begin() + index);
		EventDispatcher::sendNow(new DiffListUpdateRequest(diffscanner->getDiffs()));
		EventDispatcher::sendNow(new ShowUIMessageRequest("difference removed"));
	}
}

void Synchronizer::syncDiff(int index) {
	if (index == -1 || index >= (int) diffscanner->getDiffs()->size()) {
		EventDispatcher::sendNow(new ShowUIMessageRequest("no difference selected"));
	} else {
		Diff* diff = diffscanner->getDiffs()->at((unsigned long) index);
		//TODO execute in another thread
		DiffSync* diffSync = new DiffSync();
		diffSync->syncDiff(diff);
		delete diffSync;
		//remove synchronized diff
		diffscanner->getDiffs()->erase(diffscanner->getDiffs()->begin() + index);
		EventDispatcher::sendNow(new DiffListUpdateRequest(diffscanner->getDiffs()));
		EventDispatcher::sendNow(new ShowUIMessageRequest("difference synchronized"));
	}
}

void Synchronizer::syncAllDiffs() {
	//TODO execute in another thread
	DiffSync* diffSync = new DiffSync();
	diffSync->syncDiffs(diffscanner->getDiffs());
	delete diffSync;
	//remove synchronized diffs
	for (Diff* diff : *diffscanner->getDiffs()) {
		delete diff;
	}
	diffscanner->getDiffs()->clear();
	EventDispatcher::sendNow(new DiffListUpdateRequest(diffscanner->getDiffs()));
	EventDispatcher::sendNow(new ShowUIMessageRequest("all differences synchronized"));
}
