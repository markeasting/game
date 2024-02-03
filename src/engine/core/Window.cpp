
#include "engine/core/Window.h"
#include "engine/common/gl.h"

#include <cstdio>
#include <stdexcept>
#include <string>

Window::Window(WindowConfig config): m_config(config) {

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
        throw std::runtime_error(
            std::string("SDL could not be initialized: ") + SDL_GetError()
        );
    }

    m_window = SDL_CreateWindow(
        m_config.windowTitle,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        m_config.windowWidth,
        m_config.windowHeight,
        SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI // | SDL_WINDOW_BORDERLESS
    );

    if (!m_window) {
        throw std::runtime_error(
            std::string("Window could not be created: ") + SDL_GetError()
        );
    }

    if (m_config.fullscreen) 
        SDL_SetWindowFullscreen(m_window, SDL_WINDOW_FULLSCREEN_DESKTOP);

    /* OpenGL context */
    int context_flags = SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG;
    
    #ifdef DEBUG_BUILD
        context_flags |= SDL_GL_CONTEXT_DEBUG_FLAG;
    #endif

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, context_flags);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    #ifdef __APPLE__
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

    if (gl_context == NULL) {
        throw std::runtime_error("Failed to create OpenGL context");
    }

    SDL_GL_MakeCurrent(m_window, gl_context);

    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        throw std::runtime_error("Failed to initialize OpenGL context");
    }
    
    SDL_GL_SetSwapInterval(m_config.vsync);

    printf("[OpenGL] Device:  %s\n", glGetString(GL_RENDERER));
    printf("[OpenGL] Version: %s\n", glGetString(GL_VERSION));
    printf("[OpenGL] Window:  %d x %d\n", m_frameBufferWidth, m_frameBufferHeight);

}

void Window::swapBuffers() {
    SDL_GL_SwapWindow(m_window);
}

Window::~Window() {
    
}
