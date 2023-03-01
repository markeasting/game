#include "app/Game.h"
#include "scene/MyScene.h"
#include "util/log.h"
#include "event/EventEmitter.h"

/* Initialize static member */
EventEmitter Game::events;

Game::Game() {
    // this->registerScenes();

    init();
    initEvents();
}

void Game::init() {
    m_renderer.setupFramebuffer(m_window.m_config.windowWidth, m_window.m_config.windowHeight);
    m_camera.setProjection(m_window.m_config.windowWidth, m_window.m_config.windowHeight);

    m_camera.setPosition(glm::vec3(0.0f, 2.0f, 8.0f));
}

void Game::initEvents() {
    
    Events.on("frameBufferResize", [&] (GLFWwindow* window, int width, int height) {
        m_renderer.setupFramebuffer(width, height);
        m_camera.setProjection(width, height);
    });

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

    glfwPollEvents();

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
