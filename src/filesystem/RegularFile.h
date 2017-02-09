//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_REGUARFILE_H
#define ADBSYNC_REGUARFILE_H


#include "File.h"
#include <boost/date_time/posix_time/posix_time.hpp>

class RegularFile : public File {
public:
    RegularFile(string name);

    ~RegularFile();

    unsigned int getSize() const;

    boost::posix_time::ptime getModifiedDate() const;

    void setSize(unsigned int size);

    void setModifiedDate(boost::posix_time::ptime modifiedDate);

private:
    unsigned int size;

    boost::posix_time::ptime modifiedDate;
};


#endif //ADBSYNC_REGUARFILE_H
