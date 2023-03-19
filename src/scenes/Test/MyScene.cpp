
#include "scenes/Test/MyScene.h"

MyScene::MyScene() {

    this->addLayer("world", m_world);
    this->addLayer("overlay", m_overlay);

    m_overlay->m_active = false;

    m_state.sequence({
        ref<State>("WaitState"),
        ref<State>(State("Countdown_3", 1.0f).setGroup("countdown")),
        ref<State>(State("Countdown_2", 1.0f).setGroup("countdown")),
        ref<State>(State("Countdown_1", 1.0f).setGroup("countdown")),
        ref<State>("PLAYING"),
        ref<State>("Finish"),
    });

}

void MyScene::init() {

    // @TODO add Scene::preload() method
    // m_audio.createSource("intro_music", "test.wav");
    // m_audio.play("intro_music");

    m_camera->setPosition(glm::vec3(0.0f, 2.0f, 8.0f));

    auto lightDirection = ref<Uniform<glm::vec3>>("u_lightDirection", glm::vec3(0.5f, 0.0f, 2.0f));

    Material colorMaterial = Material("Color", {
        ref<Uniform<glm::vec4>>("u_color", glm::vec4(0.0f, 0.0f, 0.8f, 1.0f)),
    });

    auto box = ref<Mesh>(BoxGeometry(0.5f), colorMaterial);
    box->setPosition({ 2.0f, 0.5f, 0.0f });
    m_world->add(box);

    m_tetra = ref<Mesh>(TetrahedronGeometry(1.0f), colorMaterial);
    m_tetra->setPosition({ 0.0f, 1.0f, 0.0f });
    m_world->add(m_tetra);

    m_tetra->add(box);

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
    
    float oscillator = sin(time * 1.5f) / 2.0f + 0.5f;
    m_tetra->m_material->setUniform("u_color", glm::vec4(0.0f, oscillator, 0.8f, 1.0f));

    if (m_state.inGroup("countdown")) {
        m_tetra->setRotation({ time * 100.0f, 0.0f, 0.0f });
        m_tetra->m_material->setUniform("u_color", glm::vec4(oscillator, 0.0f, 0.3f, 1.0f));
    }

    if (m_state.is("PLAYING")) {

        m_tetra->setScale(sin(time * 30.0f) * 0.3f + 0.8f);

        // m_layers["overlay"]->m_active = (int) time % 2 == 0;
    }

    if (m_state.is("Finish")) {
        m_layers["world"]->m_active = false;
        m_layers["overlay"]->m_active = true;
    }
}


void MyScene::destroy() {}

