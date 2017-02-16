//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_DIFFSCANNER_H
#define ADBSYNC_DIFFSCANNER_H

#include "../diffs/Diff.h"
#include "../config/Database.h"
#include "../filesystem/LocalFS.h"
#include "../filesystem/ADB.h"
#include <vector>

using namespace std;

class DiffScanner {
public:
	DiffScanner();

	~DiffScanner();

	void scanDiffs(vector<Database*>* dbs);

	vector<Diff*>* getDiffs();

private:
	ADB* adb;

	LocalFS* localFS;

	vector<Diff*>* diffs;

	double calcProgres(int index, unsigned long all);

	void scanDirs(string localPath, string remotePath, double progressFrom, double progressTo);

	void setProgress(double p);

	File* findFile(vector<File*>* files, string name);

	template<typename T>
	bool instanceof(File* file);

	void addDiff(File* localFile, File* remoteFile, DiffType type);

	void deleteFilesList(vector<File*>* files);
};


#endif //ADBSYNC_DIFFSCANNER_H
