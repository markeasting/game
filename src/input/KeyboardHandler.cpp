
#include "input/KeyboardHandler.h"

#include "util/log.h"
#include "event/Events.h"

bool KeyboardHandler::w = false;
bool KeyboardHandler::a = false;
bool KeyboardHandler::s = false;
bool KeyboardHandler::d = false;
bool KeyboardHandler::ctrl = false;
bool KeyboardHandler::shift = false;
bool KeyboardHandler::space = false;

void KeyboardHandler::handle(SDL_Event e) {
    
    // Uint8* keys = const_cast <Uint8*> (SDL_GetKeyboardState(NULL));
    // if (keys[SDL_SCANCODE_W]) ...

    const auto isDown = e.type == SDL_KEYDOWN;
    const auto isUp = e.type == SDL_KEYUP;

    auto ev = isDown ? Events::KEYDOWN : isUp ? Events::KEYUP : "KEY_IDK";

    switch (e.key.keysym.sym) {
        case SDLK_w:        KeyboardHandler::w = isDown; Events::emit(ev, SDLK_w); break;
        case SDLK_a:        KeyboardHandler::a = isDown; Events::emit(ev, SDLK_a); break;
        case SDLK_s:        KeyboardHandler::s = isDown; Events::emit(ev, SDLK_s); break;
        case SDLK_d:        KeyboardHandler::d = isDown; Events::emit(ev, SDLK_d); break;
        case SDLK_LCTRL:    KeyboardHandler::ctrl = isDown; Events::emit(ev, SDLK_LCTRL); break;
        case SDLK_LSHIFT:   KeyboardHandler::shift = isDown; Events::emit(ev, SDLK_LSHIFT); break;
        case SDLK_SPACE:    KeyboardHandler::space = isDown; Events::emit(ev, SDLK_SPACE); break;
    }
}
