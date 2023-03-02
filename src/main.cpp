
#include "common.h"

#include "app/Game.h"
#include "geom/Mesh.h"
#include "geom/PrimitiveMeshes.h"

/* Initialize global events class */
EventEmitter Events;

Game game;

int main() {

    auto lightDirection = new Uniform<glm::vec3>("u_lightDirection", glm::vec3(0.5f, 0.0f, 2.0f));

    Material colorMaterial = Material("/shader/Color");
    
    Material phongMaterial = Material("/shader/Phong", {
        new Uniform<glm::vec3>("ambient", glm::vec3(0.2f, 0.3f, 0.3f)),
        new Uniform<glm::vec3>("diffuseAlbedo", glm::vec3(0.2f, 0.3f, 0.3f)),
        new Uniform<glm::vec3>("specularAlbedo", glm::vec3(0.2f, 0.2f, 0.2f)),
        new Uniform<glm::vec3>("rimColor", glm::vec3(0.2f, 0.3f, 0.3f)),
        new Uniform<int>("rimLightOn", 1),
        new Uniform<float>("shininess", 20.0f),
        new Uniform<float>("rimPower", 200.0f),
        lightDirection
    });

    colorMaterial.setUniform("u_color", glm::vec4(0.0f, 0.0f, 0.8f, 1.0f));

    Mesh* myMesh = new BoxMesh(1.0f, 2.0f, 1.0f);
    // myMesh->setPosition({ 3.0f, 7.0f, 0.0f });
    myMesh->setMaterial(colorMaterial);
    game.m_renderer.add(myMesh);

    // Floor //////////////////////////////////////////////////////////////////////////////////////
    Mesh* floor = new PlaneMesh(10.0f);
    floor->setRotation({ -90.0f, 0.0f, 0.0f });
    floor->setMaterial(phongMaterial);
    game.m_renderer.add(floor);

    while (game.isRunning()) {
        game.update();

        float oscillator = sin(game.getTime() * 1.5) / 2.0f + 0.5f;
        colorMaterial.setUniform("u_color", glm::vec4(0.0f, oscillator, 0.8f, 1.0f));
    }

    return 0;
}
