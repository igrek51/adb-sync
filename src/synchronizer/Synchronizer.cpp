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
#include "../events/DiffInvertedButtonClicked.h"
#include "../events/DiffPartialScanCompleted.h"

Synchronizer::Synchronizer() {
	loadConfig();
	registerEvents();
	diffs = new vector<Diff*>();
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
	EventDispatcher::registerEventObserver<DiffInvertedButtonClicked>(this);
	EventDispatcher::registerEventObserver<DiffPartialScanCompleted>(this);
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
	} else if (e->instanceof<DiffInvertedButtonClicked*>()) {
		int selectedIndex = e->cast<DiffInvertedButtonClicked*>()->selectedIndex;
		invertDiff(selectedIndex);
	} else if (e->instanceof<DiffPartialScanCompleted*>()) {
		Diff* newDiff = e->cast<DiffPartialScanCompleted*>()->newDiff;
		if (newDiff != nullptr) {
			diffs->push_back(newDiff);
		}
	}
}

void Synchronizer::scanDiffs() {
	try {
		//TODO execute in another thread
		// TODO mutexy na diffs

		diffs->clear();

		DiffScanner* diffscanner = new DiffScanner();
		diffscanner->scanDiffs(databases);

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
	if (index == -1 || index >= (int) diffs->size()) {
		EventDispatcher::sendNow(new ShowUIMessageRequest("no difference selected"));
	} else {
		Diff* diff = diffs->at((unsigned long) index);
		delete diff;
		diffs->erase(diffs->begin() + index);
		EventDispatcher::sendNow(new DiffListUpdateRequest(diffs));
		EventDispatcher::sendNow(new ShowUIMessageRequest("difference removed"));
	}
}

void Synchronizer::syncDiff(int index) {
	if (index == -1 || index >= (int) diffs->size()) {
		EventDispatcher::sendNow(new ShowUIMessageRequest("no difference selected"));
	} else {
		Diff* diff = diffs->at((unsigned long) index);
		//TODO execute in another thread
		DiffSync* diffSync = new DiffSync();
		diffSync->syncDiff(diff);
		delete diffSync;
		//remove synchronized diff
		diffs->erase(diffs->begin() + index);
		EventDispatcher::sendNow(new DiffListUpdateRequest(diffs));
		EventDispatcher::sendNow(new ShowUIMessageRequest("difference synchronized"));
	}
}

void Synchronizer::syncAllDiffs() {
	//TODO execute in another thread
	DiffSync* diffSync = new DiffSync();
	diffSync->syncDiffs(diffs);
	delete diffSync;
	//remove synchronized diffs
	for (Diff* diff : *diffs) {
		delete diff;
	}
	diffs->clear();
	EventDispatcher::sendNow(new DiffListUpdateRequest(diffs));
	EventDispatcher::sendNow(new ShowUIMessageRequest("all differences synchronized"));
}

void Synchronizer::invertDiff(int index) {
	if (index == -1 || index >= (int) diffs->size()) {
		EventDispatcher::sendNow(new ShowUIMessageRequest("no difference selected"));
	} else {
		Diff* diff = diffs->at((unsigned long) index);
		// invert synchronization direction
		diff->inverted = !diff->inverted;

		EventDispatcher::sendNow(new DiffListUpdateRequest(diffs));
		EventDispatcher::sendNow(new ShowUIMessageRequest("difference inverted"));
	}
}