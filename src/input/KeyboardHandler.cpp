
#include "input/KeyboardHandler.h"

#include "util/log.h"
#include "event/Events.h"

bool Keyboard::w = false;
bool Keyboard::a = false;
bool Keyboard::s = false;
bool Keyboard::d = false;
bool Keyboard::ctrl = false;
bool Keyboard::shift = false;
bool Keyboard::space = false;

void Keyboard::handle(SDL_Event e) {
    
    // Uint8* keys = const_cast <Uint8*> (SDL_GetKeyboardState(NULL));
    // if (keys[SDL_SCANCODE_W]) ...

    const auto isDown = e.type == SDL_KEYDOWN;
    const auto isUp = e.type == SDL_KEYUP;

    auto ev = isDown ? Events::KEYDOWN : isUp ? Events::KEYUP : "KEY_IDK";

    switch (e.key.keysym.sym) {
        case SDLK_w:        Keyboard::w = isDown; Events::emit(ev, SDLK_w); break;
        case SDLK_a:        Keyboard::a = isDown; Events::emit(ev, SDLK_a); break;
        case SDLK_s:        Keyboard::s = isDown; Events::emit(ev, SDLK_s); break;
        case SDLK_d:        Keyboard::d = isDown; Events::emit(ev, SDLK_d); break;
        case SDLK_LCTRL:    Keyboard::ctrl = isDown; Events::emit(ev, SDLK_LCTRL); break;
        case SDLK_LSHIFT:   Keyboard::shift = isDown; Events::emit(ev, SDLK_LSHIFT); break;
        case SDLK_SPACE:    Keyboard::space = isDown; Events::emit(ev, SDLK_SPACE); break;
    }
}
