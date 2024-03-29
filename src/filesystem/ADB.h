//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_ADB_H
#define ADBSYNC_ADB_H

#include "FileSystem.h"
#include "Directory.h"
#include "RegularFile.h"

class ADB : FileSystem {
public:
	ADB();

	void testADB();

	void detectDevice();

	void checkBusyBox();

	string shell(string cmd);

	bool pathExists(string path) override;

	vector<File*>* listPath(string path) override;

	void push(string localPath, string remotePath);

	void pull(string localPath, string remotePath);

	void removeFile(string path) override;

	void removeDirectory(string path) override;

protected:
	RegularFile* getRegularFileDetails(string path, string name) override;

	string escapePath(string path);

	string escapeShellPath(string path);

private:
	File* parseLsOutput(string path, string lsLine);

    Directory *parseLsDirectory(string path, string name);

    RegularFile *parseLsRegularFile(string path, string lsName);

	string busyboxDirPath;
};


#endif //ADBSYNC_ADB_H
