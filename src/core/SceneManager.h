#pragma once

#include <memory>
#include <unordered_map>

#include "scene/Scene.h"
#include "audio/Audio.h"

class SceneManager
{
public:
    SceneManager();

    void update(float time, float dt);

    unsigned int add(Ref<Scene> scene);

    void remove(unsigned int id);

    void switchTo(unsigned int id);

    Ref<Scene> getCurrentScene() const { return m_currentScene; };

    void destroy();
    
private:
    std::unordered_map<unsigned int, Ref<Scene>> m_scenes = {};
    unsigned int m_insertedSceneIdx = 0;

    Ref<Scene> m_currentScene = nullptr;

    Ref<Audio> m_audio = ref<Audio>();

};
