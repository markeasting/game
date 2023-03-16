#include "app/Game.h"
#include "scenes/MyScene.h"
#include "event/EventEmitter.h"

/* Initialize static member */
EventEmitter Game::events;

Game::Game() {
    this->registerScenes();

    Game::onResize(m_window.m_frameBufferWidth, m_window.m_frameBufferHeight);

    // Events.on("frameBufferResize", [&] (GLFWwindow* window, int width, int height) {
    //     Game::onResize(width, height);
    // });
}

Game::~Game() {
    
}

void Game::onResize(int width, int height) {
    m_renderer.setSize(width, height);
    // m_camera.setSize(width, height);

    m_sceneManager.getCurrentScene()->getCamera()->setSize(width, height);
}

void Game::registerScenes()
{
    std::shared_ptr<MyScene> gameScene = std::make_shared<MyScene>();

    unsigned int gameSceneID = m_sceneManager.add(gameScene);

    m_sceneManager.switchTo(gameSceneID);

    Game::events.emit("scene_registered");
}

void Game::update()
{

    auto scene = m_sceneManager.getCurrentScene();
    auto cam = scene->getCamera();

    while (SDL_PollEvent(&m_event)) {

        const auto e = m_event.type;

        if (e == SDL_QUIT)
            m_isRunning = false;

        if (e == SDL_KEYDOWN || e == SDL_KEYUP) 
            m_keyboard.handle(m_event);

        if (e == SDL_MOUSEBUTTONUP) {
            // @TODO handle these events in the scene
            cam->m_autoRotate = !cam->m_autoRotate;
            SDL_SetRelativeMouseMode(cam->m_autoRotate ? SDL_FALSE : SDL_TRUE);
        }
            
    }

    m_prevTime = m_time;
    m_time = SDL_GetTicks() / 1000.0f;
    m_deltaTime = m_time - m_prevTime;
    
    /* Handle scenes */
    m_sceneManager.update(m_time, m_deltaTime);
    cam->update(m_time, m_keyboard); // @TODO move to scene

    /* Handle rendering */
    m_renderer.draw(scene, cam); // @TODO give (active) scene as argument
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
