//
// Created by igrek on 04/02/17.
//

//TODO lock na kolejkę eventów przy dopisywaniu i usuwaniu, mutexy

#include "EventDispatcher.h"

EventDispatcher* EventDispatcher::instance = nullptr;

EventDispatcher* EventDispatcher::getInstance() {
    if (instance == nullptr) {
        instance = new EventDispatcher();
    }
    return instance;
}

EventDispatcher::EventDispatcher() {
    eventObservers = new map<string, std::list<IEventObserver*>*>();
    eventsQueue = new list<EventClass*>();
}

EventDispatcher::~EventDispatcher() {}

void EventDispatcher::sendLater(Event* event) {
//    Logger::debug("sending event " + EventClass::getClassName(event));
    // add event to events queue and dispatch if no event is dispatching
    getInstance()->eventsQueue->push_back(new EventClass(event));
    getInstance()->dispatchEvents();
}

void EventDispatcher::sendNow(Event* event) {
    // Avoid events queue, dispatch event immediately
    EventClass* ec = new EventClass(event);
    getInstance()->dispatch(ec);
    delete ec;
}

void EventDispatcher::unregisterEventObserver(IEventObserver* eventObserver) {
    // for every event class check if there is registered that observer
    for (std::map<string, std::list<IEventObserver*>*>::iterator it = getInstance()->eventObservers->begin();
         it != getInstance()->eventObservers->end(); ++it) {
        string eventClass = it->first;
        std::list<IEventObserver*>* observers = getInstance()->getObservers(eventClass);
        if (observers != nullptr) {
            // remove all matching observers
            std::list<IEventObserver*>::iterator it2 = observers->begin();
            while (it2 != observers->end()) {
                if ((*it2) == eventObserver) { // equals
                    it2 = observers->erase(it2);
                } else {
                    ++it2;
                }
            }
        }
    }
}

void EventDispatcher::dispatchEvents() {
    if (dispatching) return;
    dispatching = true;

    while (!eventsQueue->empty()) {
        EventClass* ec = eventsQueue->front();
        dispatch(ec);
        eventsQueue->pop_front();
        delete ec;
    }

    dispatching = false;
}

void EventDispatcher::dispatch(EventClass* ec) {
    Event* event = ec->event;
    string eventClass = ec->className;
    std::list<IEventObserver*>* observers = getObservers(eventClass);
    if (observers == nullptr || observers->empty()) {
        Logger::warn("no observer for event " + eventClass);
    }
    if (observers != nullptr) {
        for (IEventObserver* observer : *observers) {
//            Logger::debug("dispatching " + eventClass);
            observer->onEvent(event);
        }
    }
}

bool EventDispatcher::observersContains(std::list<IEventObserver*>* observers,
                                        IEventObserver* searched) {
    for (IEventObserver* observer : *observers) {
        if (observer == searched) {
            return true;
        }
    }
    return false;
}

std::list<IEventObserver*>* EventDispatcher::getObservers(string eventClass) {
    auto it = eventObservers->find(eventClass);
    if (it == eventObservers->end()) {
        return nullptr;
    } else {
        return it->second;
    }
}