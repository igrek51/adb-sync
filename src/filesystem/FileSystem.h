//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_FILESYSTEM_H
#define ADBSYNC_FILESYSTEM_H

#include <string>
#include <vector>
#include "File.h"
#include <boost/date_time/posix_time/posix_time.hpp>

using namespace std;

class FileSystem {
public:
    FileSystem();

    virtual ~FileSystem();

    virtual bool pathExists(string path) = 0;

    virtual vector<File*>* listPath(string path) = 0;

protected:
    string nextNonemptyPart(vector<string>* parts, unsigned int& index);

    boost::posix_time::ptime parseLsTime(string date, string pattern);
};


#endif //ADBSYNC_FILESYSTEM_H
