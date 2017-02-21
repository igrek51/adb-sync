//
// Created by igrek on 14/02/17.
//

#ifndef ADBSYNC_EVENTCLASS_H
#define ADBSYNC_EVENTCLASS_H

#include "Event.h"
#include <string>

using namespace std;

class EventClass {
public:
	EventClass(Event* event, string className);

	EventClass(Event* event);

	~EventClass();

	Event* event;
	string className;

	static string getClassName(Event* event);
};


#endif //ADBSYNC_EVENTCLASS_H
