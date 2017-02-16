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

time_t RegularFile::getModifiedDate() const {
    return modifiedDate;
}

void RegularFile::setSize(unsigned int size) {
    this->size = size;
}

void RegularFile::setModifiedDate(time_t modifiedDate) {
    this->modifiedDate = modifiedDate;
}
