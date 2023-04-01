
#include "physics/Collider.h"

void Collider::updateRotation(const glm::quat& rotation) { }

PlaneCollider::PlaneCollider(const glm::vec2 &size, const glm::vec3 &normal) {
    m_type = ColliderType::Plane;
    m_size = size;
    m_normal = glm::normalize(normal);
    m_normalRef = glm::normalize(normal);
}
void PlaneCollider::updateRotation(const glm::quat& rotation) {
    // normal = rotation * glm::vec3(0.0f, 0.0f, 1.0f);
    m_normal = rotation * m_normalRef;
}

SphereCollider::SphereCollider(const float &diameter) {
    m_type = ColliderType::Sphere;
    m_radius = diameter/2;
}

MeshCollider::MeshCollider(Ref<Geometry> convexGeometry) {
    m_type = ColliderType::ConvexMesh;
    setGeometry(convexGeometry);
}

MeshCollider::MeshCollider(Ref<Mesh> mesh) 
    : MeshCollider(mesh->m_geometry)
{}

void MeshCollider::setGeometry(Ref<Geometry> geometry) {

    m_vertices = geometry->m_vertexBuffer->m_data;

    if(geometry->hasIndices()) {
        m_indices = geometry->m_indexBuffer->m_data;

        for(int i = 0; i < m_indices.size(); i++) {
            if (std::find(m_uniqueIndices.begin(), m_uniqueIndices.end(), m_indices[i]) == m_uniqueIndices.end()) {
                m_uniqueIndices.push_back(m_indices[i]);
            }
        }

    } else if (m_vertices.size() > 0) {
        
        for (int i = 0; i < m_vertices.size(); i++) {
            std::cout << "Generating collider indices #INEFFICIENT" << std::endl;
            m_indices.push_back(i);
            m_uniqueIndices.push_back(i);
        }

    }
}

BoxCollider::BoxCollider(const glm::vec3 &size) 
: MeshCollider(ref<BoxGeometry>(size.x, size.y, size.z)) 
{
    // colliderType = ColliderType::Box;
    m_type = ColliderType::ConvexMesh;
}
