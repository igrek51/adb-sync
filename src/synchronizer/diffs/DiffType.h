//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_DIFFTYPE_H
#define ADBSYNC_DIFFTYPE_H

enum class DiffType {
    NO_DIRECTORY = 1,
    NO_REGULAR_FILE = 2,
    MODIFIED_CONTENT = 3,
    REDUNDANT_DIRECTORY = 4,
    REDUNDANT_REGULAR_FILE = 5,
};

#endif //ADBSYNC_DIFFTYPE_H
