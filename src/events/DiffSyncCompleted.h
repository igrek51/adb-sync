//
// Created by igrek on 16/02/17.
//

#ifndef ADBSYNC_DIFFSYNCCOMPLETED_H
#define ADBSYNC_DIFFSYNCCOMPLETED_H

#include "../dispatcher/Event.h"
#include "../diffs/Diff.h"

class DiffSyncCompleted : public Event {
public:
	Diff* diff;

	DiffSyncCompleted(Diff* diff) {
		this->diff = diff;
	}
};

#endif //ADBSYNC_DIFFSYNCCOMPLETED_H
