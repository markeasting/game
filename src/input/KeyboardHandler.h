#pragma once

// #include "common.h"

#include "util/log.h"
#include "event/Event.h"
#include "event/EventEmitter.h"

class KeyboardHandler {
public:

    bool w = false;
    bool a = false;
    bool s = false;
    bool d = false;

    bool ctrl = false;
    bool shift = false;

    bool space = false;

    void handle(SDL_Event e);
    
private:

};

extern KeyboardHandler Keyboard;
