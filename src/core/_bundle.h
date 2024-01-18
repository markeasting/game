#pragma once

#include "di/Container.h"

#include "core/Renderer.h"
#include "core/Timer.h"
#include "core/Window.h"

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
