//
// Created by igrek on 04/02/17.
//

#include "RegularFile.h"

RegularFile::RegularFile(string path, string name) : File(path, name) {
    size = 0;
}

RegularFile::~RegularFile() {

}

unsigned int RegularFile::getSize() const {
    return size;
}

boost::posix_time::ptime RegularFile::getModifiedDate() const {
    return modifiedDate;
}

void RegularFile::setSize(unsigned int size) {
    this->size = size;
}

void RegularFile::setModifiedDate(boost::posix_time::ptime modifiedDate) {
    this->modifiedDate = modifiedDate;
}
