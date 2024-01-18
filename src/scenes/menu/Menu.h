#pragma once

#include "engine/common.h"
#include "engine/scene/Scene.h"
#include "scenes/menu/layers/Interface.h"

class Menu final : public Scene {
public:

    Menu() {
        m_layers.add("interface", m_overlay);
    }

    void onClick(const SDL_MouseButtonEvent& event) override {
        Log("OMG");
        m_scene->switchTo("playground");
    }

private:

    Ref<Interface> m_overlay = ref<Interface>();

};
