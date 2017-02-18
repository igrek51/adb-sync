//
// Created by igrek on 16/02/17.
//

#ifndef ADBSYNC_DIFFSYNC_H
#define ADBSYNC_DIFFSYNC_H

#include "../diffs/Diff.h"
#include "../filesystem/ADB.h"
#include "../filesystem/LocalFS.h"
#include <vector>

using namespace std;

class DiffSync {
public:
	DiffSync();

	~DiffSync();

	void syncDiff(Diff* diff);

	void syncDiffs(vector<Diff*>* diffs);

private:
	ADB* adb;

	LocalFS* localFS;

	void setProgress(double p);
};


#endif //ADBSYNC_DIFFSYNC_H
