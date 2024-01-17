
#include "core/Game.h"
#include "scenes/playground/PlayGround.h"

int main() {

    Game game;

    /* @TODO should just have some generic 'race' scene where the map is loaded dynamically */
    // game.m_sceneManager.add("menu", ref<Menu>());
    game.m_sceneManager.add("playground", ref<PlayGround>());
    game.m_sceneManager.switchTo("playground");

    game.initialize();
    
    while (game.isRunning()) {
        game.update();
    }

    game.quit();

    return 0;
}

