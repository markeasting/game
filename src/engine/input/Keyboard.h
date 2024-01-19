#pragma once

#include <SDL2/SDL.h> 
#include <unordered_map>

struct Keyboard {

    inline static std::unordered_map<SDL_Keycode, bool> m_keys;

    static void handle(const SDL_KeyboardEvent& event) {
        m_keys[event.keysym.sym] = (event.state == SDL_PRESSED);
    }

    static bool isPressed(SDL_Keycode key) {
        auto it = m_keys.find(key);

        if (it != m_keys.end()) {
            return it->second;
        }

        return false;
    }
};

using Key = Keyboard;
