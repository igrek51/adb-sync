//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_SYNCHRONIZER_H
#define ADBSYNC_SYNCHRONIZER_H

#include "../config/Database.h"
#include "../dispatcher/IEventObserver.h"
#include "DiffScanner.h"
#include "DiffSync.h"
#include "../config/ConfigLoader.h"
#include <vector>
#include <mutex>

using namespace std;

class Synchronizer : public IEventObserver {
public:
	Synchronizer(int argc, char** argv);

	~Synchronizer();

	virtual void registerEvents() override;

	virtual void onEvent(Event* e) override;

private:
	int argc;
	char** argv;

	vector<Database*>* databases;
	vector<string>* excludedFiles;

	vector<Diff*>* diffs;
	mutex diffsMutex;

	DiffScanner* diffscanner;

	DiffSync* diffSync;

	void loadConfig();

	void scanDiffs();

	void removeDiff(int index);

	void syncDiff(int index);

	void syncAllDiffs();

	void invertDiff(int index);

	void diffSynced(Diff* diff);

	vector<Database*>* getDatabases(ConfigLoader* loader);
};


#endif //ADBSYNC_SYNCHRONIZER_H
