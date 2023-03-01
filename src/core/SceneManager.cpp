#include "common.h"
#include "core/SceneManager.h"
#include "scene/Scene.h"

// SceneManager::SceneManager() : m_scenes(0), m_curScene(0) { }
SceneManager::SceneManager() : m_scenes(0) { }

void SceneManager::update(float time, float deltaTime) {
    // if(m_curScene)
    //     m_curScene->update(deltaTime);
    //     // m_curScene->lateUpdate(deltaTime);

    // for (auto scene : m_scenes) {
    //     if (scene.second->isActive())
    //         scene.second->update(time, deltaTime);
    // }
}

void SceneManager::draw(float time, float deltaTime) {
    for (auto scene : m_scenes) {
        if (scene.second->isActive()) {
            scene.second->processInput();
            scene.second->update(time, deltaTime);
            scene.second->draw();
        }
    }
}

// template <typename T>
// unsigned int SceneManager::add(std::shared_ptr<T> scene) {
unsigned int SceneManager::add(std::shared_ptr<Scene> scene) {

    auto inserted = m_scenes.insert(std::make_pair(m_insertedSceneIdx, scene));

    inserted.first->second->create();

    return m_insertedSceneIdx++;
}

void SceneManager::remove(unsigned int id) {
    auto it = m_scenes.find(id);

    if(it != m_scenes.end()) {
        // if(m_curScene == it->second)
        //     m_curScene = nullptr;

        it->second->destroy();

        m_scenes.erase(it);
    }
}

void SceneManager::activate(unsigned int id) {
    auto it = m_scenes.find(id);

    if(it != m_scenes.end()) {
        it->second->activate();
        it->second->onActivate();
    }
}

void SceneManager::deActivate(unsigned int id) {
    auto it = m_scenes.find(id);

    if(it != m_scenes.end()) {
        it->second->deactivate();
        it->second->onDeactivate();
    }
}

void SceneManager::switchTo(unsigned int id) {
    auto it = m_scenes.find(id);

    if(it != m_scenes.end()) {
        // @TODO deactivate all other scenes

        // if(m_curScene)
        //     m_curScene->onDeactivate();
        // m_curScene = it->second;

        it->second->activate();
        it->second->onActivate();
    }
}
