//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_FILE_H
#define ADBSYNC_FILE_H

#include <string>

using namespace std;

class File {
protected:
    string name;
public:
    virtual ~File();

    string getName();

    void setName(const string& name);
};


#endif //ADBSYNC_FILE_H
