//
// Created by igrek on 04/02/17.
//

#include "File.h"

File::~File() {

}

string File::getName() {
    return name;
}

void File::setName(const string& name) {
    File::name = name;
}
