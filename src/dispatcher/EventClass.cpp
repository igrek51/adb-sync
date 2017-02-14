//
// Created by igrek on 14/02/17.
//

#include "EventClass.h"
#include <typeinfo>

EventClass::EventClass(Event* event, string className) {
    this->event = event;
    this->className = className;
}

EventClass::EventClass(Event* event) {
    this->event = event;
    this->className = getClassName(event);
}

EventClass::~EventClass() {
    delete event;
}

string EventClass::getClassName(Event* event) {
    return typeid(*event).name();
}
