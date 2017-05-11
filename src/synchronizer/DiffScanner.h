//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_DIFFSCANNER_H
#define ADBSYNC_DIFFSCANNER_H

#include "../diffs/Diff.h"
#include "../config/Database.h"
#include "../filesystem/LocalFS.h"
#include "../filesystem/ADB.h"
#include "../threads/SingleThread.h"
#include <vector>

using namespace std;

class DiffScanner : public SingleThread {
public:
	DiffScanner(vector<Database*>* dbs, vector<string>* excludedFiles);

	~DiffScanner();

	void scanDiffs();

protected:
	virtual void run() override;

private:
	ADB* adb;

	LocalFS* localFS;

	vector<Database*>* dbs;

	vector<string>* excludedFiles;

	double calcProgres(int index, unsigned long all);

	void scanDirs(string localPath, string remotePath, double progressFrom, double progressTo);

	void setProgress(double p);

	File* findFile(vector<File*>* files, string name);

	template<typename T>
	bool instanceof(File* file);

	void addDiff(File* localFile, File* remoteFile, DiffType type);

	void deleteFilesList(vector<File*>* files);

	vector<File*>* excludeIgnoredFiles(vector<File*>* files);
};


#endif //ADBSYNC_DIFFSCANNER_H
