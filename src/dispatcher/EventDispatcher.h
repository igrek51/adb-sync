//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_EVENTDISPATCHER_H
#define ADBSYNC_EVENTDISPATCHER_H

#include <iostream>
#include <map>
#include <list>
#include "IEventObserver.h"
#include "Event.h"
#include "../logger/Logger.h"

using namespace std;

class EventDispatcher {

private:

    map<string, std::list<IEventObserver*>*>* eventObservers;

    list<Event*>* eventsQueue;

    bool dispatching = false;

    EventDispatcher();

    static EventDispatcher* instance;

    static EventDispatcher* getInstance();

public:

    static void registerEventObserver(string eventClass, IEventObserver* observer);

    static void sendEvent(Event* event);

    static void unregisterEvent(string eventClass);

    static void unregisterEventObserver(IEventObserver* eventObserver);

private:
    void dispatchEvents();

    void dispatch(Event* event);

    bool observersContains(std::list<IEventObserver*>* observers, IEventObserver* searched);

    std::list<IEventObserver*>* getObservers(string eventClass);

};

#endif //ADBSYNC_EVENTDISPATCHER_H
