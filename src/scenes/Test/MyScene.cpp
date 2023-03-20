
#include "scenes/Test/MyScene.h"
#include "scene/SceneManager.h"
#include "util/Anim.h"

MyScene::MyScene() {

    m_layers.add("world", m_world);
    m_layers.add("overlay", m_overlay);

    m_overlay->m_active = false;

    m_state.sequence({
        ref<State>("WaitState"),
        ref<State>(State("Countdown_3", 1.2f).setGroup("countdown")),
        ref<State>(State("Countdown_2", 1.0f).setGroup("countdown")),
        ref<State>(State("Countdown_1", 1.0f).setGroup("countdown")),
        ref<State>("PLAYING"),
        ref<State>("Finish"),
    });

}

void MyScene::preload() {

    glm::vec3 audioPos = { 0.0f, 0.5f, 3.0f };

    m_audio->stream("intro_music", "assets/music/MOM$ - Spacial (pop off).mp3");
    m_audio->stream("race_music", "assets/music/Glidelas - Angel File [WIP] (trim).mp3");
    m_audio->stream("end_music", "assets/music/Qteku - Energy Star (trim).mp3");

    m_audio->add("Countdown_3", "assets/audio/voice/3.mp3")->setPosition(audioPos);
    m_audio->add("Countdown_2", "assets/audio/voice/2.mp3")->setPosition(audioPos);
    m_audio->add("Countdown_1", "assets/audio/voice/1.mp3")->setPosition(audioPos);
    m_audio->add("Countdown_GO", "assets/audio/voice/go.mp3")->setPosition(audioPos);
    m_audio->add("YouWin", "assets/audio/voice/you_win.mp3")->setPosition(audioPos);

    // auto src = m_audio->add("MOI", "assets/music/Qteku - Energy Star.mp3");
    // src->setPosition(audioPos);
    // m_audio->play("MOI");
}

void MyScene::init() {

    m_audio->play("intro_music");

    m_camera->setPosition(glm::vec3(0.0f, 2.0f, 8.0f));
    m_camera->m_camRadius = 8.0f;

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
                // if (m_state.is("Finish"))
                //     m_sceneManager->switchTo(0);
                // else
                    m_state.next();
            break;
        }
    });

    Events::on(Events::MOUSEUP, [&]() {
        m_camera->m_autoRotate = !m_camera->m_autoRotate;
        SDL_SetRelativeMouseMode(m_camera->m_autoRotate ? SDL_FALSE : SDL_TRUE);
    });

    Events::on(Events::STATE_CHANGE, [&](Ref<State> state) {
        Log(state->getName());

        if (state->inGroup("countdown")) {
            m_audio->fadeOut("intro_music");
            m_audio->play(state->getName());
        }

        if (state->is("Countdown_3")) {
            m_audio->play("race_music");
        }

        if (state->is("PLAYING")) {
            m_audio->play("Countdown_GO");
            // m_audio->play("race_music");
        }

        if (state->is("Finish")) {
            // m_audio->stop("race_music");
            m_audio->fadeOut("race_music");
            m_audio->play("YouWin");
            m_audio->play("end_music");
        }
    });
}

void MyScene::update(float time, float dt) {

    m_camera->update(time);
    m_state.update(time, dt); // @TODO move to base update
    m_audio->updateListener(m_camera->getPosition(), m_camera->getForward(), m_camera->getUp());
    
    // Log(m_state.getName());
    
    float oscillator = sin(time * 1.5f) / 2.0f + 0.5f;
    m_tetra->m_material->setUniform("u_color", glm::vec4(0.0f, oscillator, 0.8f, 1.0f));
    m_tetra->setRotation({ time * 100.0f, 0.0f, 0.0f });

    if (m_state.inGroup("countdown")) {
        m_tetra->m_material->setUniform("u_color", glm::vec4(oscillator, 0.0f, 0.3f, 1.0f));
    }

    if (m_state.is("PLAYING")) {
        const float beatMatch = (145.0f / 60.0f) * time * 2.0f * M_PI; 
        m_tetra->setScale(sin(beatMatch) * 0.3f + 0.8f);
        m_tetra->setRotation({ 0.0f, beatMatch * 180.0f/M_PI, 0.0f });

        m_camera->m_lookAtPos = glm::vec3(0, 1.0f, 0);
        Anim::lerp(m_camera->m_camRadius, 5.0f, 0.1f);

    }

    if (m_state.is("Finish")) {
        m_layers.get("overlay")->m_active = (int) time % 2 == 0;
        Anim::lerp(m_camera->m_camRadius, 2.5f, 0.05f);
    }
}


void MyScene::destroy() {}

