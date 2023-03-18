
#include "common.h"

#include "app/Game.h"

/* Initialize globals */
EventEmitter Events;
KeyboardHandler Keyboard;

Game game;

int main() {

    while (game.isRunning()) {
        game.update();
    }

    game.quit();

    return 0;
}
