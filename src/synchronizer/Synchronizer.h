//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_SYNCHRONIZER_H
#define ADBSYNC_SYNCHRONIZER_H

#include "../config/Database.h"
#include "../dispatcher/IEventObserver.h"
#include "DiffScanner.h"
#include <vector>

using namespace std;

class Synchronizer : public IEventObserver {
public:
    Synchronizer();

    ~Synchronizer();

    virtual void registerEvents() override;

    virtual void onEvent(Event* e) override;

private:
    vector<Database*>* databases;

	//TODO remove, create for every scan in separate thread
	DiffScanner* diffscanner;

    void loadConfig();

    void scanDiffs();

	void removeDiff(int index);

	void syncDiff(int index);

	void syncAllDiffs();
};


#endif //ADBSYNC_SYNCHRONIZER_H
