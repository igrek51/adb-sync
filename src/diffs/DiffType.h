//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_DIFFTYPE_H
#define ADBSYNC_DIFFTYPE_H

enum class DiffType {
	NO_DIRECTORY = 1, // no remote directory
	NO_REGULAR_FILE = 2, // no remote file
	DIFFERENT_CONTENT = 3, // different checksums
	DIFFERENT_SIZE = 4, // different file sizes
	REDUNDANT_DIRECTORY = 5, // no local directory
	REDUNDANT_REGULAR_FILE = 6, // no local file
};

#endif //ADBSYNC_DIFFTYPE_H
