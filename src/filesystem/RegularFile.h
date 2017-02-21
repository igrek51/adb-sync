//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_REGUARFILE_H
#define ADBSYNC_REGUARFILE_H

#include "File.h"

class RegularFile : public File {
public:
    RegularFile(string path, string name);

    ~RegularFile();

    unsigned int getSize() const;

	string getChecksum() const;

    void setSize(unsigned int size);

	void setChecksum(string checksum);

private:
    unsigned int size;

	string checksum;
};


#endif //ADBSYNC_REGUARFILE_H
