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

string RegularFile::getChecksum() const {
	return checksum;
}

void RegularFile::setSize(unsigned int size) {
    this->size = size;
}

void RegularFile::setChecksum(string checksum) {
	this->checksum = checksum;
}
