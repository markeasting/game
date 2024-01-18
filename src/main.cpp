
#include "engine/common/Container.h"

#include "engine/core/_bundle.h"
#include "engine/scene/_bundle.h"

#include "engine/core/Game.h"

#include "scenes/playground/PlayGround.h"

int main() {

    auto container = Container::instance();
    
    Engine::CoreBundle::configure({
        .window {
            .windowTitle = "MOI",
            .windowWidth = 2560,
            .windowHeight = 1440,
            .fullscreen = false,
            .vsync = false,
        },
        .renderer {
            .wireframe = false
        },
    });

    Engine::SceneManagerBundle::configure();

    /* Game class */
    container->singleton<Game, Timer, Window, Renderer, SceneManager>();

    const auto game = container->get<Game>();

    /* @TODO should just have some generic 'race' scene where the map is loaded dynamically */
    // game->m_sceneManager.add("menu", ref<Menu>());
    game->m_sceneManager->add("playground", ref<PlayGround>());
    game->m_sceneManager->switchTo("playground");

    game->initialize();
    
    while (game->isRunning()) {
        game->update();
    }

    game->quit();

    return 0;
}

