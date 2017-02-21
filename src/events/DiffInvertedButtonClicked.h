//
// Created by igrek on 16/02/17.
//

#ifndef ADBSYNC_DIFFINVERTEDBUTTONCLICKED_H
#define ADBSYNC_DIFFINVERTEDBUTTONCLICKED_H

#include "../dispatcher/Event.h"

class DiffInvertedButtonClicked : public Event {
public:
	int selectedIndex;

	DiffInvertedButtonClicked(int selectedIndex) {
		this->selectedIndex = selectedIndex;
	}
};

#endif //ADBSYNC_DIFFINVERTEDBUTTONCLICKED_H
