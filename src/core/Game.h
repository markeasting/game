#pragma once

#include "common.h"
#include "core/Window.h"
#include "scene/SceneManager.h"
#include "gfx/Renderer.h"

struct GameConfig {
    
};

class Game {
public:
    
    GameConfig m_config;

	Window m_window;

    Renderer m_renderer;

    SceneManager m_sceneManager;
    
    Game();
    ~Game();

    void initialize();
    void update();
    bool isRunning() const;
    void quit();

    float getTime() const { return m_time; }

private:

    void setSize(int width, int height);
    
    bool m_isRunning = true;
    
    float m_time = 0;
    float m_prevTime = 0;
    float m_deltaTime = 0;

};
