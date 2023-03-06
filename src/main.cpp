
#include "common_includes.h"

#include "app/Game.h"
#include "geom/Mesh.h"
#include "geom/PrimitiveMeshes.h"

/* Initialize global events class */
EventEmitter Events;

Game game;

int main() {

    auto lightDirection = ref<Uniform<glm::vec3>>("u_lightDirection", glm::vec3(0.5f, 0.0f, 2.0f));

    Material colorMaterial = Material("/shader/Color");
    
    Material phongMaterial = Material("/shader/Phong", {
        ref<Uniform<glm::vec3>>("ambient", glm::vec3(0.2f, 0.3f, 0.3f)),
        ref<Uniform<glm::vec3>>("diffuseAlbedo", glm::vec3(0.2f, 0.3f, 0.3f)),
        ref<Uniform<glm::vec3>>("specularAlbedo", glm::vec3(0.2f, 0.2f, 0.2f)),
        ref<Uniform<glm::vec3>>("rimColor", glm::vec3(0.2f, 0.3f, 0.3f)),
        ref<Uniform<int>>("rimLightOn", 1),
        ref<Uniform<float>>("shininess", 20.0f),
        ref<Uniform<float>>("rimPower", 200.0f),
        lightDirection
    });

    colorMaterial.setUniform("u_color", glm::vec4(0.0f, 0.0f, 0.8f, 1.0f));

    auto floor = ref<PlaneMesh>(10.0f);
    floor->setRotation({ -90.0f, 0.0f, 0.0f });
    floor->setMaterial(phongMaterial);
    game.m_renderer.add(floor);

    auto myMesh = ref<BoxMesh>(0.5f);
    myMesh->setPosition({ 3.0f, 2.0f, 0.0f });
    myMesh->setMaterial(colorMaterial);
    game.m_renderer.add(myMesh);

    auto tetra = ref<TetrahedronMesh>(1.0f);
    tetra->setPosition({ 0.0f, 1.0f, 0.0f });
    tetra->setMaterial(colorMaterial);
    game.m_renderer.add(tetra);

    tetra->add(myMesh);

    auto overlay = ref<PlaneMesh>(0.2f);
    // overlay->setMaterial(colorMaterial);
    overlay->setPosition({ -0.9f, -0.9f, 0.0f });
    overlay->m_useProjectionMatrix = false;
    game.m_renderer.add(overlay);

    while (game.isRunning()) {
        game.update();

        tetra->setRotation({ game.getTime() * 100.0f, 0.0f, 0.0f });

        float oscillator = sin(game.getTime() * 1.5) / 2.0f + 0.5f;
        colorMaterial.setUniform("u_color", glm::vec4(0.0f, oscillator, 0.8f, 1.0f));
    }

    return 0;
}
