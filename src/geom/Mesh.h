#pragma once

#include "common.h"
#include "gfx/Vertex.h"
#include "gfx/VertexBuffer.h"
#include "gfx/IndexBuffer.h"
#include "gfx/Material.h"
#include "geom/PrimitiveMesh.h"
#include "core/Object3D.h"

struct Mesh : public Object3D {
public: 

    VertexBuffer vertexBuffer;
    IndexBuffer indexBuffer;
    Material* material = nullptr;
    
    bool managedByRigidBody = false;

    Mesh() = default;
    Mesh(const PrimitiveMesh& primitiveMesh);
    virtual ~Mesh() = default;

    void setMaterial(Material* material);
    virtual void bind();
    virtual void unbind();
};
