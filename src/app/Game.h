#pragma once

#include "common.h"
#include "app/Window.h"
#include "core/SceneManager.h"
#include "camera/Camera.h"
#include "input/Mouse.h"
#include "gfx/Renderer.h"
#include "event/EventEmitter.h"

struct GameConfig {
    
};

class Game {
public:
    
    GameConfig m_config;

	Window m_window;

    Renderer m_renderer;
    Camera m_camera;

    static Mouse mouse;
    static EventEmitter events;
    
    Game();

    void update();
    bool isRunning() const;

    SceneManager getSceneManager() const;
    void registerScenes();

    float getTime() const { return m_time; }

private:

    void init();
    void initEvents();
    
    float m_time = 0;
    float m_prevTime = 0;
    float m_deltaTime = 0;

    SceneManager m_sceneManager;
};
