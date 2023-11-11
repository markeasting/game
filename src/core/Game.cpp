#include "core/Game.h"
#include "scenes/Test/MyScene.h"

#include "input/KeyboardHandler.h"
#include "event/Events.h"

Game::Game() {}

Game::~Game() {}

void Game::setSize(int width, int height) {
    m_renderer.setSize(width, height);

    auto currentScene = m_sceneManager.getCurrentScene();

    if (currentScene) {
        currentScene->getCamera()->setSize(width, height);
    }
}

void Game::initialize() {

    Game::setSize(m_window.m_frameBufferWidth, m_window.m_frameBufferHeight);

}

void Game::update()
{

    while (SDL_PollEvent(&m_event)) {

        const auto e = m_event.type;

        if (e == SDL_QUIT)
            m_isRunning = false;

        /** @todo change Keyboard class to be non-static / singleton / injected */
        if (e == SDL_KEYDOWN || e == SDL_KEYUP) 
            Keyboard::handle(m_event);

        /** @todo change Events class to be non-static / singleton / injected */
        if (e == SDL_MOUSEBUTTONDOWN)
            Events::emit(Events::MOUSEDOWN);

        if (e == SDL_MOUSEBUTTONUP)
            Events::emit(Events::MOUSEUP);

        if (e == SDL_KEYUP) {
            if (m_event.key.keysym.sym == SDLK_F1) {
                m_renderer.m_config.wireframe = !m_renderer.m_config.wireframe;
            }
        }
    }

    m_prevTime = m_time;
    m_time = SDL_GetTicks() / 1000.0f;
    m_deltaTime = m_time - m_prevTime;
    
    m_sceneManager.update(m_time, m_deltaTime);

    auto scene = m_sceneManager.getCurrentScene();
    auto cam = scene->getCamera();

    m_renderer.draw(scene, cam);
    m_window.swapBuffers();
}

bool Game::isRunning() const {
    return m_isRunning;
}

void Game::quit() {

    m_sceneManager.destroy();

    SDL_Quit();
}
