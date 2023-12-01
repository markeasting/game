#pragma once

#include "common.h"
#include "event/EventEmitter.h"
#include "camera/Camera.h"
#include "scene/Layer.h"
#include "state/StateManager.h"
#include "scene/LayerManager.h"

#include "audio/Audio.h"
#include "input/Keyboard.h"
#include "input/Mouse.h"
#include "input/Gamepad.h"

class SceneManager;

class Scene : public EventEmitter {
public:

    StateManager m_state;
    LayerManager m_layers;

    // @TODO dependency injection
    SceneManager* m_scene = nullptr;
    // Ref<InputManager> m_input = nullptr;      /* Injected by SceneManager */
    // Ref<Mouse>      m_mouse = nullptr;        /* Injected by SceneManager */
    // Ref<Keyboard>   m_keyboard = nullptr;     /* Injected by SceneManager */
    // Ref<Gamepad>    m_gamepad = nullptr;      /* Injected by SceneManager */

    Ref<Audio> m_audio = nullptr;   /* Injected by SceneManager */

    Scene();
    virtual ~Scene() {};

    /* Called by SceneManager */
    inline void _init() {
        for (auto const& [name, layer] : m_layers.all()) {
            layer->init();
        }
    }

    /* Overridable methods */
    virtual void load() {};
    virtual void init() {};
    virtual void update(float time, float dt) {};
    virtual void destroy() {}; // @TODO handle with destructor?

    virtual void onActivate() {};
    virtual void onDeactivate() {};

    /* Events */
    virtual void bindEvents() {};

    /** 
     * @TODO maybe employ DI for Keyboard and Mouse instances
     * But, then, we have to 'unbind' events if the scene isn't active.
     * With the current method-based solution, they are only called for the active scene
     * 
     * Should experiment with multiple (sub)scenes to see what works best
     */
    virtual void onKey(const SDL_KeyboardEvent& event) {};
    virtual void onClick(const SDL_MouseButtonEvent& event) {};
    virtual void onGamepadButton(const SDL_ControllerButtonEvent& event) {};

    Ref<Camera> getCamera() { return m_camera; }

protected:

    bool m_isActive = false;

    Ref<Camera> m_camera = ref<Camera>();
    
    // SDL_Event m_event;

};
