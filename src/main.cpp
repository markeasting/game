
#include <irrKlang.h>

#include "common.h"

#include "app/Game.h"
#include "geom/index.h"
#include "gfx/Mesh.h"

/* Initialize global events class */
EventEmitter Events;

Game game;

int main() {
    irrklang::ISoundEngine* engine = irrklang::createIrrKlangDevice();

    // engine->play2D("assets/audio/2019.wav");
    engine->play3D(
        "assets/audio/LoopDeLoop_21.wav", 
        irrklang::vec3df(0.0f, 0.5f, 3.0f),
        true
    );

    auto lightDirection = ref<Uniform<glm::vec3>>("u_lightDirection", glm::vec3(0.5f, 0.0f, 2.0f));

    Material colorMaterial = Material("Color");
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

    colorMaterial.setUniform("u_color", glm::vec4(0.0f, 0.0f, 0.8f, 1.0f));

    auto floor = ref<Mesh>(PlaneGeometry(10.0f), phongMaterial2);
    floor->setRotation({ -90.0f, 0.0f, 0.0f });
    game.m_renderer.add(floor);

    auto sphere = ref<Mesh>(SphereGeometry(1.0f), phongMaterial);
    sphere->setPosition({ 0.0f, 0.5f, 3.0f });
    game.m_renderer.add(sphere);

    auto box = ref<Mesh>(BoxGeometry(0.5f), colorMaterial);
    box->setPosition({ 2.0f, 0.5f, 0.0f });
    game.m_renderer.add(box);

    auto tetra = ref<Mesh>(TetrahedronGeometry(1.0f), colorMaterial);
    tetra->setPosition({ 0.0f, 1.0f, 0.0f });
    game.m_renderer.add(tetra);

    tetra->add(box);

    auto overlay = ref<Mesh>(PlaneGeometry(2.0f));
    Material textureMaterial = Material("Basic.vert", "BasicTextured.frag");
    textureMaterial.assignTexture("");
    overlay->setMaterial(textureMaterial);
    overlay->setPosition({ 0, -0.5f, 0 });
    overlay->setScale({ 0.3f, 0.3f, 0 });
    overlay->m_useProjectionMatrix = false;
    game.m_renderer.add(overlay);

    while (game.isRunning()) {
        game.update();

        tetra->setRotation({ game.getTime() * 100.0f, 0.0f, 0.0f });

        float oscillator = sin(game.getTime() * 1.5) / 2.0f + 0.5f;
        colorMaterial.setUniform("u_color", glm::vec4(0.0f, oscillator, 0.8f, 1.0f));

        /* Update audio listener position */
        /* Somehow it sounds better when lookAt is negated? (But then channels are flipped)*/
        auto camPos = game.m_camera.getPosition();
        auto lookAt = glm::normalize(game.m_camera.m_lookAtPos - camPos);

        Log(camPos, "pos");
        Log(lookAt, "lookAt");
        Log(" ");

        engine->setListenerPosition(
            irrklang::vec3df(camPos.x, camPos.y, camPos.z),
            irrklang::vec3df(lookAt.x, lookAt.y, lookAt.z)
        );
    }

    return 0;
}
