#pragma once

#include "common_includes.h"
#include "scene/Scene.h"

class MyScene final : public Scene
{
public:
    MyScene();

    void create() override;
    void destroy() override;

    void processInput() override;
    void update(float time, float deltaTime) override;
    void draw() override;

private:
    // Entity m_entity = Entity();
};
