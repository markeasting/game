#pragma once

#include "util/log.h"
#include "event/EventEmitter.h"

class Events {
public:

    static constexpr const char* KEYDOWN = "KEYDOWN";
    static constexpr const char* KEYUP = "KEYUP";
    static constexpr const char* MOUSEDOWN = "MOUSEDOWN";
    static constexpr const char* MOUSEUP = "MOUSEUP";

    template <typename Callback>
    static void on(const std::string &name, Callback cb) {
        _emitter.on(std::move(name), std::forward<Callback>(cb));
    }

    template <typename... Args>
    static void emit(std::string name, Args... args) {
        _emitter.emit(std::move(name), std::forward<Args>(args)...);
    }

    static EventEmitter _emitter; /* Instantiated in main.cpp */
    
private:

};
