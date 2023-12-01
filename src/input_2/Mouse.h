#pragma once

#include <SDL.h>
#include <unordered_map>
#include "common/log.h"

struct Mouse {

    std::unordered_map<unsigned int, bool> m_buttons;

    void handleMotion(const SDL_MouseMotionEvent& event) {
    }

    void handleButtonPress(const SDL_MouseButtonEvent& event) {

        const auto buttonIndex = event.button;

        m_buttons[buttonIndex] = (event.state == SDL_PRESSED); // SDL_RELEASED (event.type == SDL_MOUSEBUTTONDOWN);
    }
};
