
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

Game game;

int main() {

    alure::DeviceManager devMgr = alure::DeviceManager::getInstance();

    alure::Device dev;

    if(!dev) dev = devMgr.openPlayback();

    alure::Context ctx = dev.createContext();
    alure::Context::MakeCurrent(ctx);

    const char* audioFile = "test.wav";
    alure::Buffer buffer = ctx.getBuffer(audioFile);
    alure::Source source = ctx.createSource();
    source.play(buffer);
    std::cout<< "Playing "<< audioFile <<" ("
        << alure::GetSampleTypeName(buffer.getSampleType())<<", "
        << alure::GetChannelConfigName(buffer.getChannelConfig())<<", "
        << buffer.getFrequency()<<"hz)" <<std::endl;

    while (game.isRunning()) {
        game.update();

        if (source.isPlaying()) {
            std::cout<< "\r "<<source.getSampleOffset()<<" / "<<buffer.getLength() <<std::flush;
        }

        ctx.update();
    }

    source.destroy();
    ctx.removeBuffer(buffer);

    std::cout<<std::endl;
    alure::Context::MakeCurrent(nullptr);
    ctx.destroy();
    dev.close();

    game.quit();

    return 0;
}
