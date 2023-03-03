#pragma once

#include "common.h"
#include "input/Mouse.h"

struct WindowConfig {
    const char* windowTitle = "MOI";
    uint16_t windowWidth = 1280;
    uint16_t windowHeight = 720;
    bool vsync = true;
};

class Window {
public:

    WindowConfig m_config;

    GLFWwindow* m_window;
    Window* self = this;

    int m_windowWidth = 0;
    int m_windowHeight = 0;
    int m_frameBufferWidth = 0;
    int m_frameBufferHeight = 0;

    Window();

    void swapBuffers();     // @TODO inline these
    bool isActive() const;  // @TODO inline these

};
