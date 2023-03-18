#pragma once

#include <SDL.h>

class Keyboard {
public:

    static bool w;
    static bool a;
    static bool s;
    static bool d;

    static bool ctrl;
    static bool shift;

    static bool space;

    static void handle(SDL_Event e);

};
