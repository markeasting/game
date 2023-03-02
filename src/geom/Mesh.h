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

    Ref<VertexBuffer> m_vertexBuffer = nullptr;
    Ref<IndexBuffer> m_indexBuffer = nullptr;
    Ref<Material> m_material = nullptr;
    
    bool managedByRigidBody = false;

    Mesh();
    Mesh(const PrimitiveMesh& primitiveMesh);
    Mesh(const PrimitiveMesh& primitiveMesh, const Material& material);
    virtual ~Mesh() = default;

    void setMaterial(const Material& material);
    virtual void bind() const;
    virtual void unbind() const;
};
