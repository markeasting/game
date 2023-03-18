
#include "util/log.h"
#include "input/KeyboardHandler.h"

void KeyboardHandler::handle(SDL_Event e) {
    
    // Uint8* keys = const_cast <Uint8*> (SDL_GetKeyboardState(NULL));
    // if (keys[SDL_SCANCODE_W]) ...

    const auto isDown = e.type == SDL_KEYDOWN;
    const auto isUp = e.type == SDL_KEYUP;

    auto ev = isDown ? Event::KEYDOWN : isUp ? Event::KEYUP : "___";

    Log(ev);

    switch (e.key.keysym.sym) {
        case SDLK_w:        w = isDown; Events.emit(ev, SDLK_w); break;
        case SDLK_a:        a = isDown; Events.emit(ev, SDLK_a); break;
        case SDLK_s:        s = isDown; Events.emit(ev, SDLK_s); break;
        case SDLK_d:        d = isDown; Events.emit(ev, SDLK_d); break;
        case SDLK_LCTRL:    ctrl = isDown; Events.emit(ev, SDLK_LCTRL); break;
        case SDLK_LSHIFT:   shift = isDown; Events.emit(ev, SDLK_LSHIFT); break;
        case SDLK_SPACE:    space = isDown; Events.emit(ev, SDLK_SPACE); break;
    }
}
