#pragma once

#include "engine/common.h"
#include "engine/scene/Scene.h"
#include "engine/gfx/Mesh.h"
#include "engine/physics/PhysicsHandler.h"

#include "scenes/playground/layers/UI.h"
#include "scenes/playground/layers/World.h"
#include "entity/Car.h"

class PlayGround final : public Scene {
public:

    PlayGround();

    void load() override;
    void init() override;
    void destroy() override;

    void bindEvents() override;
    void onKey(const SDL_KeyboardEvent& event) override;
    void onClick(const SDL_MouseButtonEvent& event) override;
    void onGamepadButton(const SDL_ControllerButtonEvent& event) override {};

    void update(float time, float dt) override;

private:
    PhysicsHandler m_phys; // @TODO inject

    Ref<World> m_world = ref<World>();
    Ref<UI> m_overlay = ref<UI>();

    Ref<Mesh> m_tetra = nullptr;
    Ref<Mesh> m_skybox = nullptr;

    Ref<Car> m_player = nullptr;

    std::vector<Ref<Car>> m_opponents;

};
