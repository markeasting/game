#include "common.h"
#include "scene/SceneManager.h"
#include "scene/Scene.h"

SceneManager::SceneManager() {}

void SceneManager::update(float time, float dt) {
    if (m_currentScene == nullptr)
        return;

    m_audio->update();
    m_currentScene->update(time, dt);
}

void SceneManager::handleEvents(const SDL_Event& event) {

    if (m_currentScene == nullptr)
        return;

    // m_input->handle(event);

    switch (event.type) {

        case SDL_KEYUP:
        case SDL_KEYDOWN:
            m_currentScene->onKey(event.key);
            break;

        case SDL_MOUSEBUTTONUP:
        // case SDL_MOUSEBUTTONDOWN:
            m_currentScene->onClick(event.button);
            break;

        case SDL_CONTROLLERBUTTONUP:
            m_currentScene->onGamepadButton(event.cbutton);
            break;
    
    default:
        break;
    }

}

void SceneManager::add(std::string key, Ref<Scene> scene) {

    assert(scene != nullptr);

    Log("Adding scene: " + key);

    m_scenes[key] = scene;

    scene->m_audio = m_audio; /* Inject */
    // scene->m_input = m_input; /* Inject */
    scene->m_scene = this; /* Inject */
    
    // scene->load();
    // scene->_init();
    // scene->init();
    // scene->bindEvents();
}

void SceneManager::remove(std::string id) {
    
    /* @TODO could use at() here if we are sure the key exists */
    auto it = m_scenes.find(id);

    if(it != m_scenes.end()) {
        auto scene = it->second;
        scene->destroy();
        m_scenes.erase(it);
    }
}

void SceneManager::switchTo(std::string id) {

    /* @TODO could use at() here if we are sure the key exists */
    auto it = m_scenes.find(id);

    if (it != m_scenes.end()) {
        auto scene = it->second;

        if(m_currentScene) {
            m_currentScene->onDeactivate();
        }

        m_currentScene = scene;

        m_currentScene->load();
        m_currentScene->_init();
        m_currentScene->init();
        m_currentScene->bindEvents();
        
        m_currentScene->onActivate();
    } else {
        Log("Scene not found: " + id);
    }
}

void SceneManager::destroy() {
    m_audio->destroy();

    // @TODO more cleanup
}
