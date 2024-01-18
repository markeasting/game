
#include "scenes/playground/layers/World.h"

#include "engine/geom/index.h"
#include "engine/gfx/Mesh.h"

void World::init() {

    // @TODO share this stuff with the parent scene / other layers. 
    // E.g. using the same lights
    auto lightDirection = uniform("u_lightDirection", normalize(vec3(0.5f, 0.0f, 1.0f)));

    Material phongMaterial = Material("Phong", { lightDirection });
    Material phongMaterial2 = Material("Phong", {
        uniform("ambient", vec3(0.2f, 0.3f, 0.3f)),
        uniform("diffuseAlbedo", vec3(0.2f, 0.3f, 0.3f)),
        uniform("specularAlbedo", vec3(0.2f, 0.2f, 0.2f)),
        uniform("rimColor", vec3(0.2f, 0.3f, 0.3f)),
        uniform("rimLightOn", 1),
        uniform("shininess", 20.0f),
        uniform("rimPower", 200.0f),
        lightDirection
    });

    // auto floor = ref<Mesh>(PlaneGeometry(10.0f), phongMaterial2);
    // floor->setRotation({ -90.0f, 0.0f, 0.0f });
    // this->add(floor);

    auto sphere = ref<Mesh>(SphereGeometry(1.0f), phongMaterial);
    sphere->setPosition({ 0.0f, 0.5f, 3.0f });
    this->add(sphere);

}