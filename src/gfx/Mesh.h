#pragma once

#include "common.h"
#include "gfx/Vertex.h"
#include "gfx/Material.h"
#include "geom/Geometry.h"
#include "core/Object3D.h"

struct Mesh : public Object3D {
public: 

    Ref<Geometry> m_geometry = nullptr;
    Ref<Material> m_material = nullptr;

    bool m_managedByRigidBody = false;
    
    Mesh();
    Mesh(const Geometry& geometry);
    Mesh(const Geometry& geometry, const Material& material);
    Mesh(Ref<Geometry> geometry, const Material& material);

    virtual ~Mesh() = default;

    void setMaterial(const Material& material);
    virtual void bind() const;
    virtual void unbind() const;
    
};
