//
// Created by igrek on 16/02/17.
//

#ifndef ADBSYNC_EXECUTEDIFFBUTTONCLICKED_H
#define ADBSYNC_EXECUTEDIFFBUTTONCLICKED_H

#include "../dispatcher/Event.h"
#include "../diffs/Diff.h"

class ExecuteDiffButtonClicked : public Event {
public:
	int index;

	ExecuteDiffButtonClicked(int index) {
		this->index = index;
    }
};

#endif //ADBSYNC_EXECUTEDIFFBUTTONCLICKED_H
