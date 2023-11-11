#include "common.h"
#include "scene/SceneManager.h"
#include "scene/Scene.h"

SceneManager::SceneManager() {}

void SceneManager::update(float time, float dt) {
    if(m_currentScene == nullptr)
        return;

    m_audio->update();
    m_currentScene->update(time, dt);
}

void SceneManager::add(std::string key, Ref<Scene> scene) {

    auto inserted = m_scenes.insert(std::make_pair(key, scene));

    scene->m_audio = m_audio; /* Inject audio manager / could also do singleton? */
    // scene->m_sceneManager = ref<SceneManager>(*this); /* Inject SceneManager / could also do singleton? */
    
    scene->preload();
    scene->_init();
    scene->init();
    scene->bindEvents();
}

void SceneManager::remove(std::string id) {
    auto it = m_scenes.find(id);

    if(it != m_scenes.end()) {
        it->second->destroy();
        m_scenes.erase(it);
    }
}

void SceneManager::switchTo(std::string id) {
    auto it = m_scenes.find(id);

    if(it != m_scenes.end()) {
        if(m_currentScene) {
            m_currentScene->unBindEvents();
            m_currentScene->onDeactivate();
        }

        m_currentScene = it->second;
        m_currentScene->onActivate();
    }
}

void SceneManager::destroy() {
    m_audio->destroy();

    // @TODO more cleanup
}
