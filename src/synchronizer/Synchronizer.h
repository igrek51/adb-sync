//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_SYNCHRONIZER_H
#define ADBSYNC_SYNCHRONIZER_H

#include "../config/Database.h"
#include <vector>

using namespace std;

class Synchronizer {
public:
    Synchronizer();

    ~Synchronizer();

private:
    vector<Database*>* databases;
};


#endif //ADBSYNC_SYNCHRONIZER_H
