//
// Created by igrek on 16/02/17.
//

#ifndef ADBSYNC_SHOWUIMESSAGEREQUEST_H
#define ADBSYNC_SHOWUIMESSAGEREQUEST_H

#include "../dispatcher/Event.h"
#include <string>

using namespace std;

class ShowUIMessageRequest : public Event {
public:
	string message;

	ShowUIMessageRequest(string message) {
		this->message = message;
	}
};

#endif //ADBSYNC_SHOWUIMESSAGEREQUEST_H
