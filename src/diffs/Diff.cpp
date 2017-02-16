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

string Diff::diffTypeName(DiffType type) {
    switch (type) {
        case DiffType::NO_DIRECTORY:
            return "not existing directory";
        case DiffType::NO_REGULAR_FILE:
            return "not existing file";
        case DiffType::MODIFIED_DATE:
            return "different modify date";
        case DiffType::DIFFERENT_SIZE:
            return "different file size";
        case DiffType::REDUNDANT_DIRECTORY:
            return "reduntand directory";
        case DiffType::REDUNDANT_REGULAR_FILE:
            return "redundant file";
    }
}
