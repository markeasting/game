#pragma once

#include "scene/Scene.h"

#include <memory>
#include <unordered_map>
// #include "input/KeyboardHandler.h"

class SceneManager
{
public:
    SceneManager();

    void update(float time, float dt);

    unsigned int add(Ref<Scene> scene);

    void remove(unsigned int id);

    void switchTo(unsigned int id);

    Ref<Scene> getCurrentScene() const { return m_currentScene; };

private:
    std::unordered_map<unsigned int, Ref<Scene>> m_scenes = {};
    unsigned int m_insertedSceneIdx = 0;

    Ref<Scene> m_currentScene = nullptr;

};
