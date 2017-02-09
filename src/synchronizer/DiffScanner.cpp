//
// Created by igrek on 04/02/17.
//

#include "DiffScanner.h"
#include "../config/ConfigLoader.h"
#include "../logger/Logger.h"
#include "../utils/string_utils.h"

//TODO multithreading

DiffScanner::DiffScanner() {
    adb = new ADB();
    localFS = new LocalFS();
    diffs = new vector<Diff*>();

    // load databases from configuration
    ConfigLoader* configLoader = new ConfigLoader();
    dbs = configLoader->loadDatabases();
    delete configLoader;
}

DiffScanner::~DiffScanner() {
    for (Diff* diff : *diffs) {
        delete diff;
    }
    diffs->clear();
    delete adb;
    delete localFS;
}

void DiffScanner::scanDiffs() {
    diffs->clear();
    setProgress(0);

    adb->testADB();
    adb->detectDevice();

    if (dbs->empty()) {
        //TODO send message ui: no databases to scan
        return;
    }

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

        Logger::debug("scanning differences for database " + db->localPath);
        scanDirs(db->localPath, db->remotePath,
                 calcProgres(index, dbs->size()),
                 calcProgres(index + 1, dbs->size()));
        index++;
    }

    setProgress(1);
}

double DiffScanner::calcProgres(int index, unsigned long all) {
    return ((double) index) / all;
}

void
DiffScanner::scanDirs(string localPath, string remotePath, double progressFrom, double progressTo) {
    setProgress(progressFrom);
    vector<File*>* localFiles = localFS->listPath(localPath);
    vector<File*>* remoteFiles = adb->listPath(remotePath);

//    Logger::debug("scanDirs: " + localPath + ", " + remotePath);

    // check local files list as mirror pattern
    for (unsigned int i = 0; i < localFiles->size(); i++) {
        File* localFile = localFiles->at(i);
        File* remoteFile = findFile(remoteFiles, localFile->getName());
        if (instanceof<Directory*>(localFile)) { //directory
            if (remoteFile == nullptr) {
                addDiff(localFile, localPath, remotePath, DiffType::NO_DIRECTORY);
            } else {
                if (instanceof<RegularFile*>(remoteFile)) { //found, but it is file
                    addDiff(localFile, localPath, remotePath, DiffType::NO_DIRECTORY);
                    //TODO remove file with incompatible type
                } else {
                    double progress1 =
                            (progressTo - progressFrom) * calcProgres(i, localFiles->size()) +
                            progressFrom;
                    double progress2 =
                            (progressTo - progressFrom) * calcProgres(i + 1, localFiles->size()) +
                            progressFrom;
                    //scan subfolder
                    scanDirs(File::subfolder(localPath, localFile->getName()),
                             File::subfolder(remotePath, localFile->getName()),
                             progress1, progress2);
                }
            }
        } else if (instanceof<RegularFile*>(localFile)) { // regular file
            if (remoteFile == nullptr) {
                addDiff(localFile, localPath, remotePath, DiffType::NO_REGULAR_FILE);
            } else {
                if (instanceof<Directory*>(remoteFile)) { //found, but it is directory
                    addDiff(localFile, localPath, remotePath, DiffType::NO_REGULAR_FILE);
                    //TODO remove file with incompatible type
                } else {
                    RegularFile* localRegFile = dynamic_cast<RegularFile*>(localFile);
                    RegularFile* remoteRegFile = dynamic_cast<RegularFile*>(remoteFile);
                    //different size (block size)
                    if (localRegFile->getSize() != remoteRegFile->getSize()) {
                        addDiff(localFile, localPath, remotePath, DiffType::DIFFERENT_SIZE);
                    } else if (localRegFile->getModifiedDate() !=
                               remoteRegFile->getModifiedDate()) {

                        Logger::debug("modified date, file: " + localFile->getName() + ", local: " +
                                      time2string(localRegFile->getModifiedDate(),
                                                  "%Y-%m-%d %H:%M") +
                                      ", remote: " +
                                      time2string(remoteRegFile->getModifiedDate(),
                                                  "%Y-%m-%d %H:%M"));

                        addDiff(localFile, localPath, remotePath, DiffType::MODIFIED_DATE);
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
            if (instanceof<Directory*>(remoteFile)) { //directory
                addDiff(remoteFile, localPath, remotePath, DiffType::REDUNDANT_DIRECTORY);
            } else { //regular file
                addDiff(remoteFile, localPath, remotePath, DiffType::REDUNDANT_REGULAR_FILE);
            }
        }
    }

    // clean up
    for (unsigned int i = 0; i < localFiles->size(); i++) {
        delete localFiles->at(i);
    }
    delete localFiles;
    for (unsigned int i = 0; i < remoteFiles->size(); i++) {
        delete remoteFiles->at(i);
    }
    delete remoteFiles;
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
    //TODO send event to set progress in mainwindow
}

template<typename T>
bool DiffScanner::instanceof(File* file) {
    return dynamic_cast<T>(file) != nullptr;
}

void DiffScanner::addDiff(File* file, string localPath, string remotePath, DiffType type) {
    string fileName = file->getName();
    string localFileName = File::subfolder(localPath, fileName);
    string remoteFileName = File::subfolder(remotePath, fileName);
    Diff* diff = new Diff(localFileName, remoteFileName, type);
    diffs->push_back(diff);
}

vector<Diff*>* DiffScanner::getDiffs() {
    return diffs;
}
