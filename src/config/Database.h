//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_DATABASE_H
#define ADBSYNC_DATABASE_H

#include <string>

using namespace std;

class Database {
public:
    Database(string localPath, string remotePath);

    string localPath;
    string remotePath;
};


#endif //ADBSYNC_DATABASE_H
