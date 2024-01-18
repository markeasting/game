
#include "engine/physics/Collider.h"

#include "engine/common/glm.h"
#include "engine/geom/BoxGeometry.h"
#include <array>

void Collider::expandAABB(float scalar) { }
void Collider::updateGlobalPose(const Pose& pose) { }

PlaneCollider::PlaneCollider(const glm::vec2 &size, const glm::vec3 &normal) {
    m_type = ColliderType::PLANE;

    m_plane.normal = glm::normalize(normal);
    m_plane.constant = 0.0f; // Plane constant will be set in updateGlobalPose()
    m_plane.size = size;

}

// void PlaneCollider::updateGlobalPose(const Pose& pose) {
//     m_normal = pose.q * m_normalRef;
// }

void PlaneCollider::updateGlobalPose(const Pose& pose) {

    m_relativePosW = (pose.q * m_relativePos) + pose.p;

    m_plane.transform(m_relativePosW, pose.q);

    // m_aabb.set(min, max);
}

SphereCollider::SphereCollider(const float diameter) {
    m_type = ColliderType::SPHERE;
    m_radius = diameter/2;
}

MeshCollider::MeshCollider(Ref<Geometry> geometry, bool isConvex) {
    m_type = isConvex ? ColliderType::CONVEX_MESH : ColliderType::INEFFICIENT_MESH;
    setGeometry(geometry);
}

MeshCollider::MeshCollider(Ref<Mesh> mesh) 
    : MeshCollider(mesh->m_geometry)
{}

void MeshCollider::setGeometry(Ref<Geometry> geometry) {

    m_mesh = ref<Mesh>(*geometry);

    for (Vertex v : geometry->m_vertexBuffer->m_data) {
        m_vertices.push_back(v.position);
        m_verticesWorldSpace.push_back(v.position);
    }

    if (geometry->hasIndices()) {
        m_indices = geometry->m_indexBuffer->m_data;

        for(int i = 0; i < m_indices.size(); i++) {
            if (std::find(m_uniqueIndices.begin(), m_uniqueIndices.end(), m_indices[i]) == m_uniqueIndices.end()) {
                m_uniqueIndices.push_back(m_indices[i]);
            }
        }

        /* @TODO combine with the loop above */
        /* Note: looks a lot like updateGlobalPose */
        for (size_t i = 0; i < m_indices.size(); i += 3) {
            std::array<vec3, 4> tri;

            for (int j = 0; j < 3; ++j) {
                int index = m_indices[i + j];
                tri[j] = m_vertices[index];
            }

            /* Precompute normal */
            const vec3 edge1 = tri[1] - tri[0];
            const vec3 edge2 = tri[2] - tri[0];
            tri[3] = glm::normalize(glm::cross(edge1, edge2));

            m_triangles.push_back(tri);
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
    m_relativePosW = pos;

    for (int i = 0; i < m_vertices.size(); i++) {
        m_vertices[i] += pos;
    }
}

void MeshCollider::expandAABB(float scalar) {
    m_expanded_aabb = AABB(m_aabb);
    m_expanded_aabb.expandByScalar(scalar);
}

void MeshCollider::updateGlobalPose(const Pose& pose) {

    vec3 min = vec3(FLT_MAX);
    vec3 max = vec3(-FLT_MAX);

    m_relativePosW = (pose.q * m_relativePos) + pose.p;

    // for (int i = 0; i < m_vertices.size(); i++) {
    //     auto v = m_vertices[i];

    //     m_verticesWorldSpace[i] = (pose.q * v) + pose.p;

    //     min = glm::min(min, m_verticesWorldSpace[i]);
    //     max = glm::max(max, m_verticesWorldSpace[i]);
    // }

    // /* @TODO could probably be a lot more efficient */
    // int k = 0;
    // for (size_t i = 0; i < m_indices.size(); i += 3) {
    //     auto& tri = m_triangles[k];
        
    //     for (int j = 0; j < 3; ++j) {
    //         int index = m_indices[i + j];
    //         tri[j] = m_verticesWorldSpace[index];
    //     }

    //     /* Precompute normal */
    //     const vec3 edge1 = tri[1] - tri[0];
    //     const vec3 edge2 = tri[2] - tri[0];
    //     tri[3] = glm::normalize(glm::cross(edge1, edge2));

    //     k++;
    // }

    /* Combined loop */
    int k = 0;
    for (size_t i = 0; i < m_indices.size(); i += 3) {
        auto& tri = m_triangles[k];
        
        for (int j = 0; j < 3; ++j) {
            int index = m_indices[i + j];

            const auto v = m_vertices[index];
            m_verticesWorldSpace[index] = (pose.q * v) + pose.p;

            min = glm::min(min, m_verticesWorldSpace[index]);
            max = glm::max(max, m_verticesWorldSpace[index]);

            tri[j] = m_verticesWorldSpace[index];
        }

        /* Precompute normal */
        // const vec3 edge1 = tri[1] - tri[0];
        // const vec3 edge2 = tri[2] - tri[0];
        // tri[3] = glm::normalize(glm::cross(edge1, edge2));

        k++;
    }

    m_aabb.set(min, max);
}

vec3 MeshCollider::findFurthestPoint(const vec3& dir) const {

    vec3 maxPoint;
    float maxDist = -FLT_MAX;

    for (auto const& vertex : m_verticesWorldSpace) {
        float distance = glm::dot(vertex, dir);

        if (distance > maxDist) {
            maxDist = distance;
            maxPoint = vertex;
        }
    }

    return maxPoint;
}

BoxCollider::BoxCollider(float size) 
: MeshCollider(ref<BoxGeometry>(size, size, size)) 
{
    m_type = ColliderType::CONVEX_MESH;
}


BoxCollider::BoxCollider(const glm::vec3 &size) 
: MeshCollider(ref<BoxGeometry>(size.x, size.y, size.z)) 
{
    // colliderType = ColliderType::BOX;
    m_type = ColliderType::CONVEX_MESH;
}
