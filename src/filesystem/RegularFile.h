//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_REGUARFILE_H
#define ADBSYNC_REGUARFILE_H


#include "File.h"
#include <ctime>

class RegularFile : public File {
public:
    RegularFile(string name);

    ~RegularFile();

    unsigned int getSize() const;

    time_t getModifiedDate() const;

    void setSize(unsigned int size);

    void setModifiedDate(time_t modifiedDate);

private:
    unsigned int size;

    time_t modifiedDate;
};


#endif //ADBSYNC_REGUARFILE_H
