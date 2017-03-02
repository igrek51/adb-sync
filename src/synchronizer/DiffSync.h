//
// Created by igrek on 16/02/17.
//

#ifndef ADBSYNC_DIFFSYNC_H
#define ADBSYNC_DIFFSYNC_H

#include "../diffs/Diff.h"
#include "../filesystem/ADB.h"
#include "../filesystem/LocalFS.h"
#include "../threads/SingleThread.h"
#include <vector>

using namespace std;

class DiffSync : public SingleThread {
private:
	DiffSync();

public:
	DiffSync(Diff* diff);

	DiffSync(vector<Diff*>* diffs);

	~DiffSync();

protected:
	virtual void run() override;

private:
	ADB* adb;

	LocalFS* localFS;

	Diff* diff;

	vector<Diff*>* diffs;

	void setProgress(double p);

	void syncDiff(Diff* diff);

	void syncDiffs(vector<Diff*>* diffs);
};


#endif //ADBSYNC_DIFFSYNC_H
