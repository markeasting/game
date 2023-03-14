#pragma once

class KeyboardHandler {
public:

    // KeyboardHandler();

    bool w = false;
    bool a = false;
    bool s = false;
    bool d = false;

    bool ctrl = false;
    bool shift = false;

    void handle(SDL_Event e) {

        const auto isDown = e.type == SDL_KEYDOWN;

        switch (e.key.keysym.sym) {
            case SDLK_w:        w = isDown; break;
            case SDLK_a:        a = isDown; break;
            case SDLK_s:        s = isDown; break;
            case SDLK_d:        d = isDown; break;
            case SDLK_LCTRL:    ctrl = isDown; break;
            case SDLK_LSHIFT:   shift = isDown; break;
        }
    }
    
private:

};
