#pragma once

#include "common.h"
#include "scene/Scene.h"
#include "scenes/Test/layers/UI.h"
#include "scenes/Test/layers/World.h"

#include "geom/index.h"
#include "gfx/Mesh.h"

class MyScene final : public Scene
{
public:
    MyScene();

    void init() override;
    void destroy() override;

    void bindEvents() override;

    void update(float time, float dt) override;

private:
    Ref<World> m_world = ref<World>();
    Ref<UI> m_overlay = ref<UI>();

    Ref<Mesh> m_tetra = nullptr;

};
