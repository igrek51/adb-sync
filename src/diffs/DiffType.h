//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_DIFFTYPE_H
#define ADBSYNC_DIFFTYPE_H

enum class DiffType {
    NO_DIRECTORY = 1,
    NO_REGULAR_FILE = 2,
    MODIFIED_DATE = 3, // different last modification date
    DIFFERENT_SIZE = 4,
    REDUNDANT_DIRECTORY = 5,
    REDUNDANT_REGULAR_FILE = 6,
};

#endif //ADBSYNC_DIFFTYPE_H
