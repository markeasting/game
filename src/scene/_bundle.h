#pragma once

#include "di/Container.h"

#include "scene/SceneManager.h"

namespace SceneManagerBundle {

    inline void configure() {
        const auto c = Container::instance();

        c->singleton<SceneManager>();
    }
};
