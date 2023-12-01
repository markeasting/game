#ifndef __EVENTS_H_
#define __EVENTS_H_

#include <iostream>
#include <stdexcept>
#include <functional>
#include <typeinfo>
#include <map>

// #include <string>
#include "event/E.h"

/**
 * cxx-eventemitter
 *
 * https://github.com/heapwolf/cxx-eventemitter
 *
 * int main() {
 *
 *   EventEmitter e;
 *
 *   e.on(Event::MY_EVENT, [&](string name, int num) {
 *      // Your code
 *   });
 *
 *   e.emit(Event::MY_EVENT, "beautiful", 100);
 * }
 *
 */
class EventEmitter
{
    std::map<E, void *> events;
    std::map<E, bool> events_once;

    template <typename Callback>
    struct traits : public traits<decltype(&Callback::operator())>
    {
    };

    template <typename ClassType, typename R, typename... Args>
    struct traits<R (ClassType::*)(Args...) const>
    {
        typedef std::function<R(Args...)> fn;
    };

    template <typename Callback>
    typename traits<Callback>::fn
    to_function(Callback &cb)
    {
        return static_cast<typename traits<Callback>::fn>(cb);
    }

    int _listeners = 0;

public:
    int maxListeners = 32;

    int listeners()
    {
        return this->_listeners;
    }

    template <typename Callback>
    void on(E name, Callback cb)
    {

        auto it = events.find(name);
        if (it != events.end())
        {
            throw new std::runtime_error("duplicate listener");
        }

        if (++this->_listeners >= this->maxListeners)
        {
            std::cout
                << "warning: possible EventEmitter memory leak detected. "
                << this->_listeners
                << " listeners added. "
                << std::endl;
        };

        auto f = to_function(cb);
        auto fn = new decltype(f)(to_function(cb));
        events[name] = static_cast<void *>(fn);
    }

    template <typename Callback>
    void once(E name, Callback cb)
    {
        this->on(name, cb);
        events_once[name] = true;
    }

    void off()
    {
        events.clear();
        events_once.clear();
        this->_listeners = 0;
    }

    void off(E name)
    {

        auto it = events.find(name);

        if (it != events.end())
        {
            events.erase(it);
            this->_listeners--;

            auto once = events_once.find(name);
            if (once != events_once.end())
            {
                events_once.erase(once);
            }
        }
    }

    template <typename... Args>
    void emit(E name, Args... args)
    {

        auto it = events.find(name);
        if (it != events.end())
        {

            auto cb = events.at(name);
            auto fp = static_cast<std::function<void(Args...)> *>(cb);
            (*fp)(args...);
        }

        auto once = events_once.find(name);
        if (once != events_once.end())
        {
            this->off(name);
        }
    }

    EventEmitter(void) {}

    ~EventEmitter(void)
    {
        events.clear();
    }
};

#endif
