#pragma once

#include "engine/common/Container.h"

#include "engine/input/Gamepad.h"
#include "engine/input/Keyboard.h"
#include "engine/input/Mouse.h"

namespace InputBundle {

    struct Config {};

    inline void configure(Config config = {}) {
        const auto c = Container::instance();

        c->singleton<Gamepad>();
        c->singleton<Keyboard>();
        c->singleton<Mouse>();
    }
};
