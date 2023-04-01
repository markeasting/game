
#include "physics/Collider.h"

void Collider::updateGlobalPose(const Pose& pose) { }

PlaneCollider::PlaneCollider(const glm::vec2 &size, const glm::vec3 &normal) {
    m_type = ColliderType::Plane;
    m_size = size;
    m_normal = glm::normalize(normal);
    m_normalRef = glm::normalize(normal);
}

void PlaneCollider::updateGlobalPose(const Pose& pose) {
    m_normal = pose.q * m_normalRef;
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

    for(Vertex v : geometry->m_vertexBuffer->m_data) {
        m_vertices.push_back(v.position);
        m_verticesWorldSpace.push_back(v.position);
    }

    // @TODO this doesn't work if geometry has indices but vertices on the same locations. 
    // So, we should check EPS with all vertices here instead...
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

void MeshCollider::setRelativePos(const vec3& pos) {
    m_relativePos = pos;

    for (int i = 0; i < m_vertices.size(); i++) {
        m_vertices[i] += pos;
    }
}

void MeshCollider::updateGlobalPose(const Pose& pose) {

    // float min = vec3(std::numeric_limits<float>::infinity());
    // float max = vec3(-std::numeric_limits<float>::infinity());

    for (int i = 0; i < m_vertices.size(); i++) {
        auto v = m_vertices[i];

        m_verticesWorldSpace[i] = (pose.q * v) + pose.p;

        // min = glm::min(m_verticesWorldSpace[i]);
        // max = glm::max(m_verticesWorldSpace[i]);
    }

    // this.aabb.set(min, max);
}

vec3 MeshCollider::findFurthestPoint(const vec3& dir) {

    vec3 maxPoint;
    float maxDist = std::numeric_limits<float>::infinity();

    for (auto vertex : m_verticesWorldSpace) {
        float distance = glm::dot(vertex, dir);

        if (distance > maxDist) {
            maxDist = distance;
            maxPoint = vertex;
        }
    }

    return maxPoint;
}


BoxCollider::BoxCollider(const glm::vec3 &size) 
: MeshCollider(ref<BoxGeometry>(size.x, size.y, size.z)) 
{
    // colliderType = ColliderType::Box;
    m_type = ColliderType::ConvexMesh;
}
