#pragma once

#include "engine/common/Container.h"

#include "engine/scene/SceneManager.h"

namespace Engine {

    namespace SceneManagerBundle {

        inline void configure() {
            const auto c = Container::instance();

            c->singleton<SceneManager>();
        }
    };
}
