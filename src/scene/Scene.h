#pragma once

#include "common.h"
#include "camera/Camera.h"
#include "scene/Layer.h"
#include "state/StateManager.h"
#include "audio/Audio.h"

class Scene {
public:

    std::unordered_map<const char*, Ref<Layer>> m_layers = {};

    StateManager m_state;

    Ref<Audio> m_audio; /* Injected by SceneManager */

    Scene();
    virtual ~Scene() {};

    virtual void init() = 0;
    virtual void destroy() = 0; // @TODO handle with destructor?

    virtual void onActivate() {};
    virtual void onDeactivate() {};

    virtual void bindEvents() {};

    virtual void update(float time, float dt){};

    Ref<Camera> getCamera() { return m_camera; }

protected:

    bool m_isActive = false;

    Ref<Camera> m_camera = ref<Camera>();
    
    SDL_Event m_event;

    Ref<Layer> addLayer(const char* name, Ref<Layer> layer) {
        assert(layer != nullptr);
        m_layers[name] = layer;

        return layer;
    }

    Ref<Layer> getLayer(const char* name) {
        assert(m_layers[name] != nullptr);
        return m_layers[name];
    }

};
