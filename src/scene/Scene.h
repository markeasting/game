#pragma once

#include "common.h"
#include "camera/Camera.h"
#include "scene/Layer.h"
#include "state/StateManager.h"
#include "scene/LayerManager.h"

#include "audio/Audio.h"
#include "event/Events.h"
#include "input/KeyboardHandler.h"

// class SceneManager;

class Scene {
public:

    StateManager m_state;
    LayerManager m_layers;

    // Ref<SceneManager> m_sceneManager = nullptr; /* Injected by SceneManager */
    Ref<Audio> m_audio = nullptr; /* Injected by SceneManager */

    Scene();
    virtual ~Scene() {};

    /* Called by SceneManager */
    inline void _init() {
        for (auto& pair : m_layers.all()) {
            pair.second->init();
        }
    }

    /* Overridable methods */
    virtual void preload() = 0;
    virtual void init() = 0;
    virtual void destroy() = 0; // @TODO handle with destructor?

    virtual void bindEvents() {};

    virtual void update(float time, float dt){};

    virtual void onActivate() {};
    virtual void onDeactivate() {};

    Ref<Camera> getCamera() { return m_camera; }

protected:

    bool m_isActive = false;

    Ref<Camera> m_camera = ref<Camera>();
    
    SDL_Event m_event;

};
