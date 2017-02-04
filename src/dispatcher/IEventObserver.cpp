//
// Created by igrek on 04/02/17.
//

#include "IEventObserver.h"
#include "EventDispatcher.h"

IEventObserver::IEventObserver(){
//    registerEvents();
}

IEventObserver::~IEventObserver(){
    EventDispatcher::unregisterEventObserver(this);
}