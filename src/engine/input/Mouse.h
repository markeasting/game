#pragma once

#include <SDL2/SDL.h> 
#include <unordered_map>

struct Mouse {

    inline static std::unordered_map<unsigned int, bool> m_buttons;

    static void handleMotion(const SDL_MouseMotionEvent& event) {
    }

    static void handleButtonPress(const SDL_MouseButtonEvent& event) {

        const auto buttonIndex = event.button;

        m_buttons[buttonIndex] = (event.state == SDL_PRESSED); // SDL_RELEASED (event.type == SDL_MOUSEBUTTONDOWN);
    }
};
