#pragma once

#include "common.h"
#include "scene/Scene.h"

#include "geom/index.h"
#include "gfx/Mesh.h"

class MyScene final : public Scene
{
public:
    MyScene();

    void create() override;
    void destroy() override;

    void update(float time, float deltaTime) override;

private:
    // Entity m_entity = Entity();

    Ref<Mesh> m_tetra = nullptr;
};
