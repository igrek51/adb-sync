//
// Created by igrek on 13/02/17.
//

#ifndef ADBSYNC_DIFFREMOVEDBTNEVENT_H
#define ADBSYNC_DIFFREMOVEDBTNEVENT_H

#include "../dispatcher/Event.h"

class DiffRemovedButtonClicked : public Event {
public:
	int selectedIndex;

	DiffRemovedButtonClicked(int selectedIndex) {
		this->selectedIndex = selectedIndex;
	}
};

#endif //ADBSYNC_DIFFREMOVEDBTNEVENT_H
