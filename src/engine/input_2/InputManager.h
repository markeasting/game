#pragma once

#include "engine/common/ref.h"

#include "engine/input/Keyboard.h"
#include "engine/input/Mouse.h"
#include "engine/input/Gamepad.h"

struct InputManager {

    Ref<Mouse>      m_mouse     = ref<Mouse>();
    Ref<Keyboard>   m_keyboard  = ref<Keyboard>();
    Ref<Gamepad>    m_gamepad   = ref<Gamepad>();

    void handle(const SDL_Event& event) {
        switch (event.type) {
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                m_keyboard->handle(event.key);
                break;

            // case SDL_MOUSEMOTION:
            // case SDL_MOUSEWHEEL:
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                m_mouse->handleButtonPress(event.button);
                break;

            case SDL_CONTROLLERAXISMOTION:
                m_gamepad->handleAxisMotion(event.caxis);
                break;
            // case SDL_CONTROLLERBUTTONUP:
            case SDL_CONTROLLERBUTTONDOWN:
                m_gamepad->handleButtonPress(event.cbutton);
                break;
        }
    }

};
