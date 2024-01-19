#pragma once

#include <SDL2/SDL.h> 

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

    int m_windowWidth = 0;
    int m_windowHeight = 0;
    int m_frameBufferWidth = 0;
    int m_frameBufferHeight = 0;

    Window(WindowConfig config = {});

    ~Window();

    void swapBuffers();     // @TODO inline these

};
