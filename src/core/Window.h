#pragma once

#include "common.h"

struct WindowConfig {
    const char* windowTitle = "MOI";
    uint16_t windowWidth = 2560;
    uint16_t windowHeight = 1440;
    bool fullscreen = false;
    bool vsync = true;
};

class Window {
public:

    WindowConfig m_config;

    SDL_Window* m_window;
    Window* self = this;

    int m_windowWidth = 0;
    int m_windowHeight = 0;
    int m_frameBufferWidth = 0;
    int m_frameBufferHeight = 0;

    Window();
    ~Window();

    void swapBuffers();     // @TODO inline these

};
