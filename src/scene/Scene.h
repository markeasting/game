#pragma once

#include "common.h"
#include "camera/Camera.h"
// #include "gfx/Object3D.h" // Causes circular dependency?
#include "gfx/Mesh.h"
#include "gfx/Material.h"

class Scene {
public:

    Scene();
    // virtual ~Scene() = 0;

    // Object3D* parent = nullptr; // @TODO use weak_ptr?
    std::vector<Ref<Object3D>> m_children = {};
    std::vector<Ref<Mesh>> m_meshes = {};

    void add(Ref<Object3D> object);
    void add(Ref<Mesh> mesh);

    virtual void create() = 0;
    virtual void destroy() = 0;

    virtual void onActivate(){};
    virtual void onDeactivate(){};

    virtual void update(float time, float deltaTime){};

    Ref<Camera> getCamera() { return m_camera; }

protected:
    bool m_isActive = false;

    Material m_defaultMaterial = { "Basic" };

    Ref<Camera> m_camera = ref<Camera>();

};
