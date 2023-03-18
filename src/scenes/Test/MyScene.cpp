
#include "scenes/Test/MyScene.h"

MyScene::MyScene() {

    this->addLayer("world", m_world);
    this->addLayer("overlay", m_overlay);

    // auto initState = ref<State>("InitState");
    // auto countdownState = ref<State>("TimedState", 3.0f);
    // auto waitState = ref<State>("WaitState");

    // initState->setNext(countdownState);
    // countdownState->setNext(waitState);

    m_state.sequence({
        ref<State>("WaitState"),
        ref<State>("CountDown_3", 1.0f),
        ref<State>("CountDown_2", 1.0f),
        ref<State>("CountDown_1", 1.0f),
        ref<State>("PLAYING"),
        ref<State>("Finish"),
    });

}

void MyScene::init() {

    m_camera->setPosition(glm::vec3(0.0f, 2.0f, 8.0f));

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
    m_world->add(floor);

    auto sphere = ref<Mesh>(SphereGeometry(1.0f), phongMaterial);
    sphere->setPosition({ 0.0f, 0.5f, 3.0f });
    m_world->add(sphere);

    auto box = ref<Mesh>(BoxGeometry(0.5f), colorMaterial);
    box->setPosition({ 2.0f, 0.5f, 0.0f });
    m_world->add(box);

    m_tetra = ref<Mesh>(TetrahedronGeometry(1.0f), colorMaterial);
    m_tetra->setPosition({ 0.0f, 1.0f, 0.0f });
    m_world->add(m_tetra);

    m_tetra->add(box);

    // auto overlay = ref<Mesh>(PlaneGeometry(2.0f));
    // Material textureMaterial = Material("Basic.vert", "BasicTextured.frag");
    // textureMaterial.assignTexture("assets/texture/uv_test.jpg", "texture1");
    // textureMaterial.assignTexture("assets/texture/checker_purple.png", "texture2");
    // overlay->setMaterial(textureMaterial);
    // overlay->setPosition({ 0, -0.5f, 0 });
    // overlay->setScale({ 0.3f, 0.3f, 0 });
    // overlay->m_useProjectionMatrix = false;
    // this->getLayer("overlay")->add(overlay);

}

/**
 * @TODO look into bindEvents()
 * These listeners are currently bound for the entire lifetime of the game
 * Maybe make the event listeners specific per scene / unbind everything on changing scenes
 */
void MyScene::bindEvents() {

    Events::on(Events::KEYUP, [&](SDL_KeyCode key) {
        switch (key) {
            case SDLK_SPACE:
                m_state.next();
            break;
        }
    });

    Events::on(Events::MOUSEUP, [&]() {
        m_camera->m_autoRotate = !m_camera->m_autoRotate;
        SDL_SetRelativeMouseMode(m_camera->m_autoRotate ? SDL_FALSE : SDL_TRUE);
    });

}

void MyScene::update(float time, float dt) {

    m_camera->update(time);
    m_state.update(time, dt); // @TODO move to base update
    
    // Log(m_state.getCurrentStateName());
    
    float oscillator = sin(time * 1.5) / 2.0f + 0.5f;
    m_tetra->m_material->setUniform("u_color", glm::vec4(0.0f, oscillator, 0.8f, 1.0f));

    if (m_state.is("PLAYING")) {

        m_tetra->setRotation({ time * 100.0f, 0.0f, 0.0f });
        m_tetra->m_material->setUniform("u_color", glm::vec4(oscillator, 0.0f, 0.3f, 1.0f));

        m_layers["overlay"]->m_active = (int) time % 2 == 0;
    } 

}


void MyScene::destroy() {}

