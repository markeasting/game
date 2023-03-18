#pragma once

#include "common.h"
#include "scene/Scene.h"
#include "scenes/Test/MainUI.h"

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
    Ref<Layer> m_world = ref<Layer>();
    Ref<MainUI> m_overlay = ref<MainUI>();

    Ref<Mesh> m_tetra = nullptr;

};
