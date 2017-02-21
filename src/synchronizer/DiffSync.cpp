//
// Created by igrek on 16/02/17.
//

#include "DiffSync.h"
#include "../dispatcher/EventDispatcher.h"
#include "../events/ProgressUpdated.h"

DiffSync::DiffSync() {
	adb = new ADB();
	localFS = new LocalFS();
}

DiffSync::~DiffSync() {
	delete adb;
	delete localFS;
}

void DiffSync::syncDiff(Diff* diff) {
	if (!diff->inverted) { // synchronize from local to remote
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
	} else { // synchronize from remote to local
		switch (diff->type) {
			case DiffType::NO_DIRECTORY: // no remote directory
				localFS->removeDirectory(diff->localFile);
				break;
			case DiffType::NO_REGULAR_FILE: // no remote file
				localFS->removeFile(diff->localFile);
				break;
			case DiffType::DIFFERENT_CONTENT: // different checksums
			case DiffType::DIFFERENT_SIZE: // different file sizes
				// remove remote file
				localFS->removeFile(diff->localFile);
				// copy file from local
				adb->pull(diff->localFile, diff->remoteFile);
				break;
			case DiffType::REDUNDANT_DIRECTORY: // no local directory
				adb->pull(diff->localFile, diff->remoteFile);
				break;
			case DiffType::REDUNDANT_REGULAR_FILE: // no local file
				adb->pull(diff->localFile, diff->remoteFile);
				break;
		}
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