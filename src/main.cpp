
#include "common.h"

#include "app/Game.h"

#include "AL/alure2.h"

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

    // @TODO move this to Audio class
    alure::DeviceManager devMgr = alure::DeviceManager::getInstance();
    alure::Device dev;

    if(!dev) dev = devMgr.openPlayback();
    alure::Context ctx = dev.createContext();
    alure::Context::MakeCurrent(ctx);

    Audio audio(ctx);

    audio.createSource("moi", "test.wav");
    audio.play("moi");

    Game game;
    
    while (game.isRunning()) {
        game.update();

        ctx.update();
    }

    audio.destroy();

    // source.destroy();
    // ctx.removeBuffer(buffer);

    alure::Context::MakeCurrent(nullptr);
    ctx.destroy();
    dev.close();

    game.quit();

    return 0;
}
