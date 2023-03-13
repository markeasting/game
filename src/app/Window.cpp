#include "app/Window.h"
#include "event/EventEmitter.h"

Window::Window() {

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf(
            "SDL could not be initialized!\n"
            "SDL_Error: %s\n", 
            SDL_GetError()
        );
    }

// #if defined linux && SDL_VERSION_ATLEAST(2, 0, 8)
//     // Disable compositor bypass
//     if(!SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0"))
//     {
//         printf("SDL can not disable compositor bypass!\n");
//         return 0;
//     }
// #endif

    // Create window
    m_window = SDL_CreateWindow(
        "Basic C SDL project",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        m_config.windowWidth,
        m_config.windowHeight,
        SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI // | SDL_WINDOW_BORDERLESS
    );

    if(!m_window) {
        printf(
            "Window could not be created!\n"
            "SDL_Error: %s\n", 
            SDL_GetError()
        );
        std::exit(EXIT_FAILURE);
    }

    int context_flags = SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;
    
    // #ifdef _DEBUG
        context_flags |= SDL_GL_CONTEXT_DEBUG_FLAG;
    // #endif

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, context_flags);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    #ifdef __APPLE__
        // glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        // glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
        // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        // glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    #else
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    #endif

    SDL_GLContext gl_context = SDL_GL_CreateContext(m_window);

    // SDL_GetWindowSize
    SDL_GL_GetDrawableSize(m_window, &m_frameBufferWidth, &m_frameBufferHeight);

    if(gl_context == NULL) {
        Log("Failed to create OpenGL context");
        std::exit(EXIT_FAILURE);
    }

    SDL_GL_MakeCurrent(m_window, gl_context);

    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        Log("Failed to initialize OpenGL context.", LogLevel::ERROR);
        std::exit(EXIT_FAILURE);
    }
    
    SDL_GL_SetSwapInterval(m_config.vsync);

    printf("[OpenGL] Device:  %s\n", glGetString(GL_RENDERER));
    printf("[OpenGL] Version: %s\n", glGetString(GL_VERSION));
    printf("[OpenGL] Window:  %d x %d\n", m_frameBufferWidth, m_frameBufferHeight);

    // glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
    //     auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));

    //     assert(self != nullptr);

    //     self->m_windowWidth = width;
    //     self->m_windowHeight = height;
    
    //     Events.emit("windowResize", window, width, height);

    // });

    // glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
    //     auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));

    //     assert(self != nullptr);

    //     self->m_frameBufferWidth= width;
    //     self->m_frameBufferHeight = height;
    
    //     Events.emit("frameBufferResize", window, width, height);

    // });
    
    // // @TODO move to InputHandler
    // glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
    //     Events.emit("click", window, button, action, mods);
    // });

}

void Window::swapBuffers() {
    SDL_GL_SwapWindow(m_window);
}

Window::~Window() {
    
}
