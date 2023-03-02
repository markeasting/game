#include "app/Window.h"
#include "event/EventEmitter.h"

Window::Window() {

    glfwInit();

    #ifdef __APPLE__
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #endif
    
    // glfwWindowHint(GLFW_DECORATED, GLFW_FALSE); // Disables border / close button etc. 
    // glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);
    // glfwWindowHint(GLFW_CENTER_CURSOR, GLFW_FALSE);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

    m_window = glfwCreateWindow(
        m_config.windowWidth, 
        m_config.windowHeight, 
        "MOI", 
        NULL, 
        NULL
    );

    glfwMakeContextCurrent(m_window);

    if (!m_window) {
        Log("GLFW window could not be created.", LogLevel::ERROR);
        glfwTerminate();
    }
    
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        Log("Failed to initialize OpenGL context.", LogLevel::ERROR);
        glfwTerminate();
    }

    if(m_config.vsync)
	    glfwSwapInterval(1);
    else 
	    glfwSwapInterval(0);

    printf("Initialized with OpenGL %d.%d.\n", GLVersion.major, GLVersion.minor);
    printf("Supported GLSL version is %s.\n", (char*) glGetString(GL_SHADING_LANGUAGE_VERSION));

    glfwSetWindowSizeCallback(m_window, Window::onWindowResize);
    glfwSetFramebufferSizeCallback(m_window, Window::onFrameBufferResize);
    glfwSetMouseButtonCallback(m_window, Window::onMouseButtonClick);

}

void Window::swapBuffers() {
    glfwSwapBuffers(m_window);
}

void Window::onWindowResize(GLFWwindow* window, int width, int height) {

    Events.emit("windowResize", window, width, height);

}

void Window::onFrameBufferResize(GLFWwindow* window, int width, int height) {

    Events.emit("frameBufferResize", window, width, height);
    
}

void Window::onMouseButtonClick(GLFWwindow* window, int button, int action, int mods) {
    
    Events.emit("click", window, button, action, mods);

}

bool Window::isActive() const {
    return !glfwWindowShouldClose(m_window);
}
