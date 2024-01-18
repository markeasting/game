
#include "core/Renderer.h"
#include "core/_bundle.h"
#include "di/Container.h"

#include "core/Window.h"
#include "scene/SceneManager.h"

#include "core/Game.h"
#include "scene/_bundle.h"
#include "scenes/playground/PlayGround.h"

int main() {

    auto container = Container::instance();
    
    CoreBundle::configure({
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

    SceneManagerBundle::configure();

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

