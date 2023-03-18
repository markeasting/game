
#include "scenes/Test/layers/World.h"

#include "geom/index.h"
#include "gfx/Mesh.h"

void World::init() {

    // @TODO share this stuff with the parent scene / other layers. 
    // E.g. using the same lights
    auto lightDirection = ref<Uniform<glm::vec3>>("u_lightDirection", glm::vec3(0.5f, 0.0f, 2.0f));

    Material phongMaterial = Material("Phong", { lightDirection });
    Material phongMaterial2 = Material("Phong", {
        ref<Uniform<glm::vec3>>("ambient", glm::vec3(0.2f, 0.3f, 0.3f)),
        ref<Uniform<glm::vec3>>("diffuseAlbedo", glm::vec3(0.2f, 0.3f, 0.3f)),
        ref<Uniform<glm::vec3>>("specularAlbedo", glm::vec3(0.2f, 0.2f, 0.2f)),
        ref<Uniform<glm::vec3>>("rimColor", glm::vec3(0.2f, 0.3f, 0.3f)),
        ref<Uniform<int>>("rimLightOn", 1),
        ref<Uniform<float>>("shininess", 20.0f),
        ref<Uniform<float>>("rimPower", 200.0f),
        lightDirection
    });

    auto floor = ref<Mesh>(PlaneGeometry(10.0f), phongMaterial2);
    floor->setRotation({ -90.0f, 0.0f, 0.0f });
    this->add(floor);

    auto sphere = ref<Mesh>(SphereGeometry(1.0f), phongMaterial);
    sphere->setPosition({ 0.0f, 0.5f, 3.0f });
    this->add(sphere);

}