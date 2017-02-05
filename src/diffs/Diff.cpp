//
// Created by igrek on 04/02/17.
//

#include "Diff.h"

Diff::Diff(string localFile, string remoteFile, DiffType type) {
    this->localFile = localFile;
    this->remoteFile = remoteFile;
    this->type = type;
    this->reversed = false;
}
