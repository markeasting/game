#include "geom/PrimitiveMeshes.h"
#include "geom/GeometryGenerator.h"
#include "geom/PrimitiveMesh.h"

BoxMesh::BoxMesh(float size) : Mesh() {
    PrimitiveMesh geometry = GeometryGenerator::Box(size, size, size);
    m_vertexBuffer->setVertices(geometry.vertices);
    m_indexBuffer->setIndices(geometry.indices);
}

BoxMesh::BoxMesh(float width, float height, float depth) : Mesh() {
    PrimitiveMesh geometry = GeometryGenerator::Box(width, height, depth);
    m_vertexBuffer->setVertices(geometry.vertices);
    m_indexBuffer->setIndices(geometry.indices);
}

PlaneMesh::PlaneMesh(float size) : Mesh() {
    PrimitiveMesh geometry = GeometryGenerator::Plane(size, size);
    m_vertexBuffer->setVertices(geometry.vertices);
    m_indexBuffer->setIndices(geometry.indices);
}

PlaneMesh::PlaneMesh(float length, float width) : Mesh() {
    PrimitiveMesh geometry = GeometryGenerator::Plane(length, width);
    m_vertexBuffer->setVertices(geometry.vertices);
    m_indexBuffer->setIndices(geometry.indices);
}

SphereMesh::SphereMesh(float size, unsigned int segments) : Mesh() {
    PrimitiveMesh geometry = GeometryGenerator::Sphere(size, segments);
    m_vertexBuffer->setVertices(geometry.vertices);
    m_indexBuffer->setIndices(geometry.indices);
}

TetrahedronMesh::TetrahedronMesh(float size) : Mesh() {
    PrimitiveMesh geometry = GeometryGenerator::Tetrahedron(size);
    m_vertexBuffer->setVertices(geometry.vertices);
    m_indexBuffer->setIndices(geometry.indices);
}

ArrowMesh::ArrowMesh(float size) : Mesh() {
    PrimitiveMesh geometry = GeometryGenerator::Arrow(size);
    m_vertexBuffer->setVertices(geometry.vertices);
    m_indexBuffer->setIndices(geometry.indices);
}
