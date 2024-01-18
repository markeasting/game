#pragma once

#include "engine/common/Container.h"

#include "PhysicsHandler.h"

namespace Engine {
        
    namespace PhysicsBundle {

        struct Config {};

        inline void configure(Config config = {}) {
            const auto c = Container::instance();

            c->transient<PhysicsHandler>();
        }
    };
}
