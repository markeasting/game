#pragma once

#include "engine/common.h"
#include "engine/gfx/Mesh.h"
#include "engine/gfx/Material.h"
#include "engine/physics/RigidBody.h"

class Layer {
public:

    std::vector<Ref<Object3D>> m_children = {};
    std::vector<Ref<Mesh>> m_meshes = {};

    bool m_active = true;

    Layer();
    virtual ~Layer() {};

    void add(Ref<Object3D> object);
    void add(Ref<Mesh> mesh);
    void add(Ref<RigidBody> body);

    virtual void init() {};
    virtual void destroy() {}; // @TODO handle with destructor?

    virtual void onActivate() {};
    virtual void onDeactivate() {};

    virtual void update(float time, float dt) {};

private:

    Material m_defaultMaterial = { "Basic" }; // @TODO move / make static?
};
