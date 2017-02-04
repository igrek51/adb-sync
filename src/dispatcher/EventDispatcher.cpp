//
// Created by igrek on 04/02/17.
//

//TODO lock na kolejkę eventów przy dopisywaniu i usuwaniu

#include "EventDispatcher.h"
#include <typeinfo>

EventDispatcher* EventDispatcher::instance = nullptr;

EventDispatcher* EventDispatcher::getInstance() {
    if (instance == nullptr) {
        instance = new EventDispatcher();
    }
    return instance;
}

EventDispatcher::EventDispatcher() {
    eventObservers = new map<string, std::list<IEventObserver*>*>();
    eventsQueue = new list<Event*>();
}

template<typename T>
void EventDispatcher::registerEventObserver(IEventObserver* observer) {
//    string s = boost::typeindex::type_id_with_cvr<decltype(T)>().pretty_name();
    string eventClass = typeid(T).name();
    std::list<IEventObserver*>* observers = getInstance()->getObservers(eventClass);
    if (observers == nullptr) {
        observers = new list<IEventObserver*>();
    }
    if (!getInstance()->observersContains(observers, observer)) {
        observers->push_back(observer);
    }
    getInstance()->eventObservers->at(eventClass) = observers;
}

void EventDispatcher::sendEvent(Event* event) {
    getInstance()->eventsQueue->push_back(event);
    getInstance()->dispatchEvents();
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

void EventDispatcher::unregisterEventObserver(IEventObserver* eventObserver) {
    for (std::map<string, std::list<IEventObserver*>*>::iterator it = getInstance()->eventObservers->begin();
         it != getInstance()->eventObservers->end(); ++it) {
        string eventClass = it->first;
        std::list<IEventObserver*>* observers = getInstance()->getObservers(eventClass);
        if (observers != nullptr) {
            std::list<IEventObserver*>::iterator it2 = observers->begin();
            while (it2 != observers->end()) {
                if ((*it2) == eventObserver) { // equals
                    observers->erase(it2);
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
        Event* e = eventsQueue->front();
        dispatch(e);
        eventsQueue->pop_front();
        delete e;
    }

    dispatching = false;
}

void EventDispatcher::dispatch(Event* event) {
    std::list<IEventObserver*>* observers = getObservers(event->className());
    if (observers == nullptr || observers->empty()) {
        Logger::warn("no observer for event " + event->className());
    }
    if (observers != nullptr) {
        for (IEventObserver* observer : *observers) {
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