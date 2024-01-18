#pragma once

#include "common.h"

#include "scene/Scene.h"
#include "audio/Audio.h"

// #include "input/InputManager.h"

class SceneManager {
public:

    SceneManager();

    void update(float time, float dt);

    void handleEvents(const SDL_Event& event);

    /**
     * @TODO maybe return a WeakRef here, instead of having to pass strings around
     */
    void add(std::string key, Ref<Scene> scene);

    void remove(std::string key);

    void switchTo(std::string key);

    Ref<Scene> getCurrentScene() const { return m_currentScene; };

    void destroy();
    
private:
    std::unordered_map<std::string, Ref<Scene>> m_scenes = {};

    Ref<Scene> m_currentScene = nullptr;

    Ref<Audio> m_audio = ref<Audio>();

    // InputManager m_input;
    // Ref<InputManager> m_input = ref<InputManager>();

};
