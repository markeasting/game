#include "common.h"
#include "core/SceneManager.h"
#include "scene/Scene.h"

SceneManager::SceneManager() : m_scenes(0), m_currentScene(0) { }
// SceneManager::SceneManager() : m_scenes(0) { }

void SceneManager::update(float time, float deltaTime) {
    if(m_currentScene)
        m_currentScene->update(time, deltaTime);

    // for (auto scene : m_scenes) {
    //     if (scene.second->isActive())
    //         scene.second->update(time, deltaTime);
    // }
}

unsigned int SceneManager::add(Ref<Scene> scene) {

    auto inserted = m_scenes.insert(std::make_pair(m_insertedSceneIdx, scene));

    inserted.first->second->create();

    return m_insertedSceneIdx++;
}

void SceneManager::remove(unsigned int id) {
    auto it = m_scenes.find(id);

    if(it != m_scenes.end()) {
        it->second->destroy();
        m_scenes.erase(it);
    }
}

// void SceneManager::activate(unsigned int id) {
//     auto it = m_scenes.find(id);

//     // if(it != m_scenes.end()) {
//     //     // if(m_curScene == it->second)
//     //     //     m_curScene = nullptr;

//     //     it->second->destroy();
//     //     m_scenes.erase(it);
//     // }

//     if(it != m_scenes.end()) {
//         it->second->onActivate();
//     }
// }

// void SceneManager::deActivate(unsigned int id) {
//     auto it = m_scenes.find(id);

//     if(it != m_scenes.end()) {
//         it->second->onDeactivate();
//     }
// }

void SceneManager::switchTo(unsigned int id) {
    auto it = m_scenes.find(id);

    if(it != m_scenes.end()) {
        if(m_currentScene)
            m_currentScene->onDeactivate();

        m_currentScene = it->second;
        m_currentScene->onActivate();
    }
}
