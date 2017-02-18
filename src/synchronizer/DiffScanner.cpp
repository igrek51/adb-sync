//
// Created by igrek on 04/02/17.
//

#include "DiffScanner.h"
#include "../logger/Logger.h"
#include "../dispatcher/EventDispatcher.h"
#include "../events/ProgressUpdated.h"
#include "../events/ShowUIMessageRequest.h"
#include "../events/DiffPartialScanCompleted.h"
#include "../events/DiffListUpdateRequest.h"

//TODO multithreading
//TODO possibility to stop scanning
//TODO synchronizing single files from configuration

DiffScanner::DiffScanner() {
	adb = new ADB();
	localFS = new LocalFS();
	diffs = new vector<Diff*>();
}

DiffScanner::~DiffScanner() {
	for (Diff* diff : *diffs) {
		delete diff;
	}
	diffs->clear();
	delete adb;
	delete localFS;
}

void DiffScanner::scanDiffs(vector<Database*>* dbs) {
	diffs->clear();
	setProgress(0);
	EventDispatcher::sendNow(new DiffListUpdateRequest(diffs));

	adb->testADB();
	adb->detectDevice();

	if (dbs->empty()) {
		EventDispatcher::sendNow(new ShowUIMessageRequest("no database defined to scan"));
		return;
	}

	EventDispatcher::sendNow(new ShowUIMessageRequest("Scanning for differences..."));

	int index = 0;
	for (Database* db : *dbs) {

		// check if directories exists
		if (!localFS->pathExists(db->localPath)) {
			Logger::warn("Local path does not exist: " + db->localPath);
			continue;
		}
		if (!adb->pathExists(db->remotePath)) {
			Logger::warn("Remote path does not exist: " + db->remotePath);
			continue;
		}

		Logger::debug("scanning for differences in database " + db->localPath);
		scanDirs(db->localPath, db->remotePath,
				 calcProgres(index, dbs->size()),
				 calcProgres(index + 1, dbs->size()));
		index++;
	}

	// scanning completed
	EventDispatcher::sendNow(new DiffPartialScanCompleted(nullptr));
}

double DiffScanner::calcProgres(int index, unsigned long all) {
	return ((double) index) / all;
}

void DiffScanner::scanDirs(string localPath, string remotePath, double progressFrom,
						   double progressTo) {
	setProgress(progressFrom);
	vector<File*>* localFiles = localFS->listPath(localPath);
	vector<File*>* remoteFiles = adb->listPath(remotePath);

	Logger::debug("scanning Dir: " + localPath);

	// check local files list as mirror pattern
	for (unsigned int i = 0; i < localFiles->size(); i++) {
		File* localFile = localFiles->at(i);
		File* remoteFile = findFile(remoteFiles, localFile->getName());
		if (instanceof<Directory*>(localFile)) { //directory
			if (remoteFile == nullptr) {
				remoteFile = new Directory(remotePath, localFile->getName());
				addDiff(localFile, remoteFile, DiffType::NO_DIRECTORY);
				delete remoteFile;
			} else {
				if (instanceof<RegularFile*>(remoteFile)) { //found, but it is a file
					Logger::warn(
							"incompatible file types: " + localPath + "/" + localFile->getName());
					remoteFile = new Directory(remotePath, localFile->getName());
					addDiff(localFile, remoteFile, DiffType::NO_DIRECTORY);
					delete remoteFile;
				} else {
					double progress1 = (progressTo - progressFrom) *
									   calcProgres(i, localFiles->size()) + progressFrom;
					double progress2 = (progressTo - progressFrom) *
									   calcProgres(i + 1, localFiles->size()) + progressFrom;
					//scan subfolder recursively
					scanDirs(File::subfolder(localPath, localFile->getName()),
							 File::subfolder(remotePath, localFile->getName()),
							 progress1, progress2);
				}
			}
		} else if (instanceof<RegularFile*>(localFile)) { // regular file
			if (remoteFile == nullptr) {
				remoteFile = new RegularFile(remotePath, localFile->getName());
				addDiff(localFile, remoteFile, DiffType::NO_REGULAR_FILE);
				delete remoteFile;
			} else {
				if (instanceof<Directory*>(remoteFile)) { //found, but it is a directory
					Logger::warn(
							"incompatible file types: " + localPath + "/" + localFile->getName());
					remoteFile = new RegularFile(remotePath, localFile->getName());
					addDiff(localFile, remoteFile, DiffType::NO_REGULAR_FILE);
					delete remoteFile;
				} else {
					RegularFile* localRegFile = dynamic_cast<RegularFile*>(localFile);
					RegularFile* remoteRegFile = dynamic_cast<RegularFile*>(remoteFile);
					//different size (block size)
					if (localRegFile->getSize() != remoteRegFile->getSize()) {
						addDiff(localFile, remoteFile, DiffType::DIFFERENT_SIZE);
					} else if (localRegFile->getModifiedDate() !=
							   remoteRegFile->getModifiedDate()) {
						//TODO fix modify date setting
//						addDiff(localFile, remoteFile, DiffType::MODIFIED_DATE);
					}
				}
			}
		}
		setProgress(
				(progressTo - progressFrom) * calcProgres(i, localFiles->size()) + progressFrom);
	}
	// check for redundant files from remote files list
	for (unsigned int i = 0; i < remoteFiles->size(); i++) {
		File* remoteFile = remoteFiles->at(i);
		if (findFile(localFiles, remoteFile->getName()) == nullptr) { //if not found
			File* localFile = new Directory(localPath, remoteFile->getName());
			if (instanceof<Directory*>(remoteFile)) { //directory
				addDiff(localFile, remoteFile, DiffType::REDUNDANT_DIRECTORY);
			} else { //regular file
				addDiff(localFile, remoteFile, DiffType::REDUNDANT_REGULAR_FILE);
			}
			delete localFile;
		}
	}

	// clean up
	deleteFilesList(localFiles);
	deleteFilesList(remoteFiles);
}

File* DiffScanner::findFile(vector<File*>* files, string name) {
	//TODO quick search from set
	for (File* file : *files) {
		if (file->getName() == name) {
			return file;
		}
	}
	return nullptr;
}

void DiffScanner::setProgress(double p) {
	EventDispatcher::sendNow(new ProgressUpdated(p));
}

template<typename T>
bool DiffScanner::instanceof(File* file) {
	return dynamic_cast<T>(file) != nullptr;
}

void DiffScanner::addDiff(File* localFile, File* remoteFile, DiffType type) {
	string localFileName = localFile->getFullPathName();
	string remoteFileName = remoteFile->getFullPathName();
	Diff* diff = new Diff(localFileName, remoteFileName, type);

	//save also modify dates
	if (type == DiffType::MODIFIED_DATE || type == DiffType::DIFFERENT_SIZE) {
		RegularFile* localRegFile = dynamic_cast<RegularFile*>(localFile);
		RegularFile* remoteRegFile = dynamic_cast<RegularFile*>(remoteFile);
		diff->localModifyTime = localRegFile->getModifiedDate();
		diff->remoteModifyTime = remoteRegFile->getModifiedDate();
	} else if (type == DiffType::NO_REGULAR_FILE) {
		RegularFile* localRegFile = dynamic_cast<RegularFile*>(localFile);
		diff->localModifyTime = localRegFile->getModifiedDate();
	} else if (type == DiffType::REDUNDANT_REGULAR_FILE) {
//		RegularFile* remoteRegFile = dynamic_cast<RegularFile*>(remoteFile);
//		diff->remoteModifyTime = remoteRegFile->getModifiedDate();
	}

	diffs->push_back(diff);

	EventDispatcher::sendNow(new DiffPartialScanCompleted(diff));
}

vector<Diff*>* DiffScanner::getDiffs() {
	return diffs;
}

void DiffScanner::deleteFilesList(vector<File*>* files) {
	for (File* file : *files) {
		delete file;
	}
	delete files;
}
