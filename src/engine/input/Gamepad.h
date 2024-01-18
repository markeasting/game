#pragma once

#include <SDL.h>
#include <array>
#include <cassert>
#include <unordered_map>

/**
 * @todo deadzone
 * @todo sensitivity
 * 
 * SDL_GameControllerGetJoystick();
 * SDL_JoystickNameForIndex()
 */
class Gamepad {
public:

    inline static SDL_GameController* m_controller;
    // inline static SDL_Joystick *joystick;

    inline static bool m_active = true;

    /**
     * 0 - Left x-axis
     * 1 - Left y-axis
     * 2 - Right x-axis
     * 3 - Right y-axis
     */
    inline static std::array<float, 3> m_axes = {0};

    inline static std::unordered_map<SDL_GameControllerButton, bool> m_buttons;

    static void init() {
        m_controller = SDL_GameControllerOpen(0);
    }

    /* Store axis values mapped to [-1.0, 1.0]*/
    static void handleAxisMotion(const SDL_ControllerAxisEvent& event) {

        assert(event.axis <= 3 && "Axis ID out of array bounds");

        float normalized = static_cast<float>(event.value) / INT16_MAX;

        m_axes[event.axis] = normalized;
    }

    static void handleButtonPress(const SDL_ControllerButtonEvent& event) {
        m_buttons[(SDL_GameControllerButton) event.button] = (event.state == SDL_PRESSED);
    }

    static bool isButtonPressed(SDL_GameControllerButton key) {
        auto it = m_buttons.find(key);

        if (it != m_buttons.end()) {
            return it->second;
        }

        return false;
    }
};
