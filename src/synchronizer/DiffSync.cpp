//
// Created by igrek on 16/02/17.
//

#include "DiffSync.h"
#include "../dispatcher/EventDispatcher.h"
#include "../events/ProgressUpdated.h"

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
	//TODO handle reversed diffs
	switch (diff->type) {
		case DiffType::NO_DIRECTORY:
			//push whole directory
			adb->push(diff->localFile, diff->remoteFile);
			break;
		case DiffType::NO_REGULAR_FILE:
			// copy file
			adb->push(diff->localFile, diff->remoteFile);
			break;
		case DiffType::DIFFERENT_CONTENT:
		case DiffType::DIFFERENT_SIZE:
			// remove remote file
			adb->removeFile(diff->remoteFile);
			// copy file from local
			adb->push(diff->localFile, diff->remoteFile);
			break;
		case DiffType::REDUNDANT_DIRECTORY:
			adb->removeDirectory(diff->remoteFile);
			break;
		case DiffType::REDUNDANT_REGULAR_FILE:
			adb->removeFile(diff->remoteFile);
			break;
	}
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