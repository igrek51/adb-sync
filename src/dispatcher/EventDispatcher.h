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
#include "EventClass.h"
#include <typeinfo>

using namespace std;

class EventDispatcher {

private:

    map<string, std::list<IEventObserver*>*>* eventObservers;

    list<EventClass*>* eventsQueue;

    bool dispatching = false;

    EventDispatcher();

    static EventDispatcher* instance;

    static EventDispatcher* getInstance();

public:

    ~EventDispatcher();

    template<typename T>
    static void registerEventObserver(IEventObserver* observer);

    static void sendLater(Event* event);

    static void sendNow(Event* event);

    template<typename T>
    static void unregisterEvent();

    static void unregisterEventObserver(IEventObserver* eventObserver);

private:
    void dispatchEvents();

    void dispatch(EventClass* ec);

    bool observersContains(std::list<IEventObserver*>* observers, IEventObserver* searched);

    std::list<IEventObserver*>* getObservers(string eventClass);

};

template<typename T>
void EventDispatcher::registerEventObserver(IEventObserver* observer) {
//    string s = boost::typeindex::type_id_with_cvr<decltype(T)>().pretty_name();
    string eventClass = typeid(T).name();
//    Logger::debug("registering observer for events " + eventClass);
    std::list<IEventObserver*>* observers = getInstance()->getObservers(eventClass);
    if (observers == nullptr) {
        observers = new list<IEventObserver*>();
        getInstance()->eventObservers->insert(make_pair(eventClass, observers));
    }
    if (!getInstance()->observersContains(observers, observer)) {
        observers->push_back(observer);
    }
}

template<typename T>
void EventDispatcher::unregisterEvent() {
    string eventClass = typeid(T).name();
    std::list<IEventObserver*>* observers = getInstance()->getObservers(eventClass);
    if (observers != nullptr) {
        observers->clear();
    }
    auto it = getInstance()->eventObservers->find(eventClass);
    if (it != getInstance()->eventObservers->end()) {
        getInstance()->eventObservers->erase(it);
    }
}

#endif //ADBSYNC_EVENTDISPATCHER_H
