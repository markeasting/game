
#include "common.h"

#include "core/Game.h"
#include "scenes/Test/MyScene.h"

/**
 * What’s the "static initialization order 'fiasco' (problem)"?
 * 
 * https://isocpp.org/wiki/faq/ctors#static-init-order
 * 
 * Fix:
 * https://isocpp.org/wiki/faq/ctors#static-init-order-on-first-use-members
 * 
 */
EventEmitter Events::_emitter;

int main() {

    Game game;

    Ref<MyScene> gameScene = ref<MyScene>();

    game.m_sceneManager.add("myScene", gameScene);
    game.m_sceneManager.switchTo("myScene");
    game.initialize();
    
    while (game.isRunning()) {
        game.update();
    }

    game.quit();

    return 0;
}

