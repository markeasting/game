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

    Window();

    void swapBuffers();     // @TODO inline these
    bool isActive() const;  // @TODO inline these

    static void onWindowResize(GLFWwindow* window, int width, int height);
    static void onFrameBufferResize(GLFWwindow* window, int width, int height);
    // static void onMouseMove(GLFWwindow* window, double xpos, double ypos);
    static void onMouseButtonClick(GLFWwindow* window, int button, int action, int mods);


};
