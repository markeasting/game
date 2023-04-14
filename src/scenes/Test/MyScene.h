#pragma once

#include "common.h"
#include "scene/Scene.h"
#include "scenes/Test/layers/UI.h"
#include "scenes/Test/layers/World.h"

#include "geom/index.h"
#include "gfx/Mesh.h"

#include "physics/PhysicsHandler.h"
#include "physics/RigidBody.h"

#include "entity/Car.h"

class MyScene final : public Scene
{
public:
    MyScene();

    void preload() override;
    void init() override;
    void destroy() override;

    void bindEvents() override;

    void update(float time, float dt) override;

private:
    PhysicsHandler m_phys;

    Ref<World> m_world = ref<World>();
    Ref<UI> m_overlay = ref<UI>();

    Ref<Mesh> m_tetra = nullptr;
    // Ref<RigidBody> m_player = nullptr;
    Ref<Car> m_player = nullptr;

};
