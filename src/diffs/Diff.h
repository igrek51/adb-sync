//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_DIFF_H
#define ADBSYNC_DIFF_H

#include <string>
#include "DiffType.h"

using namespace std;

class Diff {
public:
	Diff(string localFile, string remoteFile, DiffType type);

	string localFile;

	string remoteFile;

	DiffType type;
	/// if diff direction is reversed (synchronzation from remote to local)
	bool inverted;

	string typeName();
};


#endif //ADBSYNC_DIFF_H
