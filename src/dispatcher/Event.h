//
// Created by igrek on 04/02/17.
//

#ifndef ADBSYNC_EVENT_H
#define ADBSYNC_EVENT_H

class Event {

public:
    virtual ~Event();

    template<typename T>
    bool instanceof();

    template<typename T>
    T cast();
};

template<typename T>
bool Event::instanceof() {
    return dynamic_cast<T>(this) != nullptr;
}

template<typename T>
T Event::cast() {
    return dynamic_cast<T>(this);
}

#endif //ADBSYNC_EVENT_H
