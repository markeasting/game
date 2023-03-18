
#include "common.h"

#include "app/Game.h"

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

Game game;

int main() {

    while (game.isRunning()) {
        game.update();
    }

    game.quit();

    return 0;
}
