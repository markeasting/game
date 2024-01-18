#pragma once

#include "engine/core/Window.h"
#include "engine/core/Timer.h"
#include "engine/scene/SceneManager.h"
#include "engine/core/Renderer.h"

struct GameConfig {
    
};

class Game {
public:
    
    GameConfig m_config;

	Ref<Timer> m_time = nullptr;
	Ref<Window> m_window = nullptr;
    Ref<Renderer> m_renderer = nullptr;
    Ref<SceneManager> m_sceneManager = nullptr;
    
    Game(
        Ref<Timer> timer, 
        Ref<Window> window, 
        Ref<Renderer> renderer, 
        Ref<SceneManager> sceneManager
    ) : 
        m_time(timer), 
        m_window(window), 
        m_renderer(renderer), 
        m_sceneManager(sceneManager) 
    {}

    ~Game();

    void initialize();
    void update();
    bool isRunning() const;
    void quit();

private:

    void setSize(int width, int height);
    
    bool m_isRunning = true;

};
