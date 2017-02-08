//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_DIFFTYPE_H
#define ADBSYNC_DIFFTYPE_H

enum class DiffType {
    NO_DIRECTORY,
    NO_REGULAR_FILE,
    MODIFIED_DATE, // different last modification date
    DIFFERENT_SIZE,
    REDUNDANT_DIRECTORY,
    REDUNDANT_REGULAR_FILE,
};

#endif //ADBSYNC_DIFFTYPE_H
