#include "engine/core/Game.h"

#include <SDL.h>

#include "engine/input/Mouse.h"
#include "engine/input/Keyboard.h"
#include "engine/input/Gamepad.h"

/** Used in update() / SDL_PollEvent() */
static SDL_Event _event; 

Game::~Game() {}

void Game::setSize(int width, int height) {
    m_renderer->setSize(width, height);

    auto currentScene = m_sceneManager->getCurrentScene();

    if (currentScene) {
        currentScene->getCamera()->setSize(width, height);
    }
}

void Game::initialize() {

    Gamepad::init(); // @TODO inject input class

    setSize(m_window->m_frameBufferWidth, m_window->m_frameBufferHeight);
}

void Game::update()
{
    const auto [time, dt] = m_time->update();

    while (SDL_PollEvent(&_event)) {

        const auto e = _event.type;

        if (e == SDL_QUIT) {
            m_isRunning = false;
            break;
        }

        m_sceneManager->handleEvents(_event);

        switch (_event.type) {
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                Keyboard::handle(_event.key);
                break;

            // case SDL_MOUSEMOTION:
            // case SDL_MOUSEWHEEL:
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                Mouse::handleButtonPress(_event.button);
                break;

            case SDL_CONTROLLERAXISMOTION:
                Gamepad::handleAxisMotion(_event.caxis);
                break;
            case SDL_CONTROLLERBUTTONUP:
            case SDL_CONTROLLERBUTTONDOWN:
                Gamepad::handleButtonPress(_event.cbutton);
                break;
        }

        if (e == SDL_KEYUP) {
            if (_event.key.keysym.sym == SDLK_F1) {
                m_renderer->m_config.wireframe = !m_renderer->m_config.wireframe;
            }
        }
    }
    
    m_sceneManager->update(time, dt);

    auto scene = m_sceneManager->getCurrentScene();
    auto cam = scene->getCamera();

    m_renderer->draw(scene, cam);
    m_window->swapBuffers();
}

bool Game::isRunning() const {
    return m_isRunning;
}

void Game::quit() {
    
    // This destroys Audio as well
    // @TODO seems kinda weird
    m_sceneManager->destroy(); 

    // @TODO more cleanup

    SDL_Quit();
}
