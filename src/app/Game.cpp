#include "app/Game.h"
#include "scene/MyScene.h"
#include "event/EventEmitter.h"
#include "input/Mouse.h"

/* Initialize static member */
EventEmitter Game::events;

Game::Game() {
    // this->registerScenes();

    m_camera.setPosition(glm::vec3(0.0f, 2.0f, 8.0f));

    Game::onResize(m_window.m_frameBufferWidth, m_window.m_frameBufferHeight);

    Events.on("frameBufferResize", [&] (GLFWwindow* window, int width, int height) {
        Game::onResize(width, height);
    });
}

Game::~Game() {
    
}

void Game::onResize(int width, int height) {
    m_renderer.setupFramebuffer(width, height);
    m_camera.setProjection(m_renderer.m_frameBufferWidth, m_renderer.m_frameBufferHeight);
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

    // @TODO move to InputHandler
    glfwPollEvents();
    Mouse &mouse = Mouse::instance();
    mouse.update(m_window.m_window);

    m_prevTime = m_time;
    m_time = glfwGetTime();
    m_deltaTime = m_time - m_prevTime;
    
    /* Handle scenes */
    m_sceneManager.update(m_time, m_deltaTime);
    m_camera.update(m_time);

    /* Handle rendering */
    m_renderer.draw(&m_camera); // @TODO give (active) scene as argument
    m_window.swapBuffers();
}

SceneManager Game::getSceneManager() const
{
    return m_sceneManager;
}

bool Game::isRunning() const
{
    return m_window.isActive();
}
