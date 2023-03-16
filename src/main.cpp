
#include "common.h"

#include "app/Game.h"

/* Initialize global events class */
EventEmitter Events;

Game game;

int main() {

    while (game.isRunning()) {
        game.update();
    }

    game.quit();

    return 0;
}
