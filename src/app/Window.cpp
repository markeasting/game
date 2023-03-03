#include "app/Window.h"
#include "event/EventEmitter.h"

Window::Window() {
    
    glfwInit();

    #ifdef __APPLE__
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);
    #endif
    
    // glfwWindowHint(GLFW_DECORATED, GLFW_FALSE); // Disables border / close button etc. 
    // glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);
    // glfwWindowHint(GLFW_CENTER_CURSOR, GLFW_FALSE);
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

    m_window = glfwCreateWindow(
        m_config.windowWidth, 
        m_config.windowHeight, 
        "MOI", 
        NULL, 
        NULL
    );

    glfwSetWindowUserPointer(m_window, self);
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

    glfwGetWindowSize(m_window, &m_windowWidth, &m_windowHeight);
    glfwGetFramebufferSize(m_window, &m_frameBufferWidth, &m_frameBufferHeight);

    glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
        auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));

        assert(self != nullptr);

        self->m_windowWidth = width;
        self->m_windowHeight = height;
    
        Events.emit("windowResize", window, width, height);

    });

    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
        auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));

        assert(self != nullptr);

        self->m_frameBufferWidth= width;
        self->m_frameBufferHeight = height;
    
        Events.emit("frameBufferResize", window, width, height);

    });
    
    // @TODO move to InputHandler
    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
        Events.emit("click", window, button, action, mods);
    });

}

void Window::swapBuffers() {
    glfwSwapBuffers(m_window);
}

bool Window::isActive() const {
    return !glfwWindowShouldClose(m_window);
}
