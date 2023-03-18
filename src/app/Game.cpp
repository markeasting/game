#include "app/Game.h"
#include "scenes/Test/MyScene.h"

#include "input/KeyboardHandler.h"

/* Initialize static member */
// EventEmitter Game::events;

Game::Game() {
    this->registerScenes();

    Game::setSize(m_window.m_frameBufferWidth, m_window.m_frameBufferHeight);

}

Game::~Game() {
    
}

void Game::setSize(int width, int height) {
    m_renderer.setSize(width, height);
    m_sceneManager.getCurrentScene()->getCamera()->setSize(width, height);
}

void Game::registerScenes()
{
    Ref<MyScene> gameScene = ref<MyScene>();

    unsigned int gameSceneID = m_sceneManager.add(gameScene);

    m_sceneManager.switchTo(gameSceneID);

}

void Game::update()
{

    while (SDL_PollEvent(&m_event)) {

        const auto e = m_event.type;

        if (e == SDL_QUIT)
            m_isRunning = false;

        if (e == SDL_KEYDOWN || e == SDL_KEYUP) 
            Keyboard::handle(m_event);

        if (e == SDL_MOUSEBUTTONDOWN)
            Events::emit(Events::MOUSEDOWN);

        if (e == SDL_MOUSEBUTTONUP)
            Events::emit(Events::MOUSEUP);
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

SceneManager Game::getSceneManager() const {
    return m_sceneManager;
}

bool Game::isRunning() const {
    return m_isRunning;
}

void Game::quit() const {
    SDL_Quit();
}
