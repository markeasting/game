#pragma once

#include "engine/common/Container.h"

#include "engine/core/Renderer.h"
#include "engine/core/Timer.h"
#include "engine/core/Window.h"

namespace Engine {
    
    namespace CoreBundle {

        struct Config {
            WindowConfig window;
            RendererConfig renderer;
        };

        inline void configure(Config config = {}) {
            const auto c = Container::instance();

            c->singleton<Timer>();

            c->singleton<Renderer>(config.renderer);

            c->singleton<Window>(config.window);
        }
    };
}