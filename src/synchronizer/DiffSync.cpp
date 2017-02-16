//
// Created by igrek on 16/02/17.
//

#include "DiffSync.h"
#include "../dispatcher/EventDispatcher.h"
#include "../events/ProgressUpdated.h"
#include "../system/CommandExecutor.h"

//TODO multithreading

DiffSync::DiffSync() {
	adb = new ADB();
	localFS = new LocalFS();
}

DiffSync::~DiffSync() {
	delete adb;
	delete localFS;
}

void DiffSync::syncDiff(Diff* diff) {
	vector<string>* commands = generateSyncCommands(diff);
	for (string command : *commands) {
		CommandExecutor::execute(command);
	}
	delete commands;
}

void DiffSync::syncDiffs(vector<Diff*>* diffs) {
	for (unsigned int i = 0; i < diffs->size(); i++) {
		setProgress((double) (i) / diffs->size());
		Diff* diff = diffs->at(i);
		syncDiff(diff);
	}
	setProgress(1);
}

void DiffSync::setProgress(double p) {
	EventDispatcher::sendNow(new ProgressUpdated(p));
}

vector<string>* DiffSync::generateSyncCommands(Diff* diff) {
	vector<string>* cmds = new vector<string>();
	//TODO handle reversed diffs
	switch (diff->type) {
		case DiffType::NO_DIRECTORY:
			cmds->push_back("not existing directory");
//		case DiffType::NO_REGULAR_FILE:
//			return "not existing file";
//		case DiffType::MODIFIED_DATE:
//			return "different modify date";
//		case DiffType::DIFFERENT_SIZE:
//			return "different file size";
//		case DiffType::REDUNDANT_DIRECTORY:
//			return "reduntand directory";
//		case DiffType::REDUNDANT_REGULAR_FILE:
//			return "redundant file";
	}
	return cmds;
}
