#include "geom/PrimitiveMeshes.h"
#include "geom/GeometryGenerator.h"
#include "geom/PrimitiveMesh.h"
#include "geom/BoxGeometry.h"

// BoxMesh::BoxMesh(float size) : Mesh() {
//     PrimitiveMesh geometry = GeometryGenerator::Box(size, size, size);
//     // m_vertexBuffer->setVertices(geometry.vertices);
//     // m_indexBuffer->setIndices(geometry.indices);
//     m_geometry = ref<Geometry>(geometry);
// }

// BoxMesh::BoxMesh(float width, float height, float depth) : Mesh() {
//     // PrimitiveMesh geometry = GeometryGenerator::Box(width, height, depth);
//     // // m_vertexBuffer->setVertices(geometry.vertices);
//     // // m_indexBuffer->setIndices(geometry.indices);
//     // m_geometry = ref<Geometry>(geometry);
//     m_geometry = ref<BoxGeometry>(width, height, depth);
// }

// PlaneMesh::PlaneMesh(float size) : Mesh() {
//     PrimitiveMesh geometry = GeometryGenerator::Plane(size, size);
//     // m_vertexBuffer->setVertices(geometry.vertices);
//     // m_indexBuffer->setIndices(geometry.indices);
//     m_geometry = ref<Geometry>(geometry);
// }

// PlaneMesh::PlaneMesh(float length, float width) : Mesh() {
//     PrimitiveMesh geometry = GeometryGenerator::Plane(length, width);
//     // m_vertexBuffer->setVertices(geometry.vertices);
//     // m_indexBuffer->setIndices(geometry.indices);
//     m_geometry = ref<Geometry>(geometry);
// }

// SphereMesh::SphereMesh(float size, unsigned int segments) : Mesh() {
//     PrimitiveMesh geometry = GeometryGenerator::Sphere(size, segments);
//     // m_vertexBuffer->setVertices(geometry.vertices);
//     // m_indexBuffer->setIndices(geometry.indices);
//     m_geometry = ref<Geometry>(geometry);
// }

// TetrahedronMesh::TetrahedronMesh(float size) : Mesh() {
//     PrimitiveMesh geometry = GeometryGenerator::Tetrahedron(size);
//     // m_vertexBuffer->setVertices(geometry.vertices);
//     // m_indexBuffer->setIndices(geometry.indices);
//     m_geometry = ref<Geometry>(geometry);
// }

// ArrowMesh::ArrowMesh(float size) : Mesh() {
//     PrimitiveMesh geometry = GeometryGenerator::Arrow(size);
//     // m_vertexBuffer->setVertices(geometry.vertices);
//     // m_indexBuffer->setIndices(geometry.indices);
//     m_geometry = ref<Geometry>(geometry);
// }
