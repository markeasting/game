
#include "scenes/Test/MyScene.h"
#include "physics/Collider.h"
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

    vec3 audioPos = { 0.0f, 0.5f, 3.0f };

    // m_audio->stream("intro_music", "assets/music/MOM$ - Spacial (pop off).mp3");
    // m_audio->stream("race_music", "assets/music/Glidelas - Angel File [WIP] (trim).mp3");
    // m_audio->stream("end_music", "assets/music/Qteku - Energy Star (trim).mp3");

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

    // m_audio->play("intro_music");

    m_camera->setPosition(vec3(0.0f, 2.0f, 8.0f));
    m_camera->m_camRadius = 8.0f;

    auto lightDirection = ref<Uniform<vec3>>("u_lightDirection", vec3(0.5f, 0.0f, 2.0f));

    Material phongMaterial = Material("Phong", { lightDirection });
    Material colorMaterial = Material("Color", {
        ref<Uniform<vec4>>("u_color", vec4(0.0f, 0.0f, 0.8f, 1.0f)),
    });

    auto cube = ref<Mesh>(BoxGeometry(0.5f), colorMaterial);
        cube->setPosition({ 2.0f, 0.5f, 0.0f });
        m_world->add(cube);

    m_tetra = ref<Mesh>(TetrahedronGeometry(1.0f), colorMaterial);
        m_tetra->setPosition({ 0.0f, 1.0f, 0.0f });
        m_world->add(m_tetra);
        m_tetra->add(cube);

    /* Physics world */
    auto car = ref<Mesh>(Geometry(obj::loadModelFromFile("assets/objects/car/car.obj")), phongMaterial);
    auto colliderSize = vec3(1.42f, 0.95f, 3.0f);

    m_player = ref<RigidBody>(
            ref<BoxCollider>(colliderSize),
            car
        );
        m_player->setBox(colliderSize);
        m_player->setColliderOffset(vec3(0, 0.48f, -0.12));
        m_player->setPosition({ 4.0f, 2.0f, -3.0f });
        // m_player->makeStatic();
        m_world->add(m_player);
        m_phys.add(m_player);

    for (size_t i = 0; i < 4; i++) {
        auto box = ref<RigidBody>(
            ref<BoxCollider>(1.0f), 
            ref<Mesh>(BoxGeometry(1.0f), colorMaterial)
        );
        box->setPosition({ 4.0f, 0.5f + i * 1.2f, 0.0f });
        box->setBox(vec3(1.0f));
        m_world->add(box);
        m_phys.add(box);
    }

    Material floorMaterial = Material("Basic.vert", "BasicTextured.frag");
    floorMaterial.assignTexture("assets/texture/asphalt.jpg", "texture1");

    auto floor = ref<RigidBody>(
            ref<PlaneCollider>(vec2(30.0f, 30.0f)),
            ref<Mesh>(PlaneGeometry(30.0f, true), floorMaterial)
        );
        floor->makeStatic();
        m_world->add(floor);
        m_phys.add(floor); // idk, but order seems to matter here :P

    // auto floor = ref<RigidBody>(
    //         ref<Mesh>(PlaneGeometry(30.0f, true), floorMaterial)
    //     );
    //     floor->makeStatic();
    //     m_world->add(floor);
    //     m_phys.add(floor);

    m_phys.init();
    for (auto const& mesh : m_phys.m_debugMeshes) 
        m_world->add(mesh);

}

/**
 * @TODO look into bindEvents()
 * These listeners are currently bound for the entire lifetime of the game
 * Maybe make the event listeners specific per scene / unbind everything on changing scenes
 */
void MyScene::bindEvents() {

    Events::on(Events::KEYDOWN, [&](SDL_KeyCode key) {
        switch (key) {
            case SDLK_i:
                m_player->applyForce(vec3(0, 250.0f, 0));
            break;
        }
    });

    Events::on(Events::KEYUP, [&](SDL_KeyCode key) {
        switch (key) {
            case SDLK_SPACE:
                m_state.next();
            break;
        }
    });

    SDL_SetRelativeMouseMode(SDL_TRUE);
    Events::on(Events::MOUSEUP, [&]() {
        m_camera->m_autoRotate = !m_camera->m_autoRotate;
        SDL_SetRelativeMouseMode(m_camera->m_autoRotate ? SDL_FALSE : SDL_TRUE);
    });

    Events::on(Events::STATE_CHANGE, [&](Ref<State> state) {
        Log(state->getName());

        if (state->inGroup("countdown")) {
            // m_audio->fadeOut("intro_music");
            m_audio->play(state->getName());
        }

        if (state->is("Countdown_3")) {
            // m_audio->play("race_music");
        }

        if (state->is("PLAYING")) {
            m_audio->play("Countdown_GO");
        }

        if (state->is("Finish")) {
            // m_audio->fadeOut("race_music");
            // m_audio->play("end_music");
            m_audio->play("YouWin");
        }
    });
}

void MyScene::update(float time, float dt) {
    
    // if (Keyboard::space)
        m_phys.update(dt);
    
    /* Update colors */
    float osc = sin(time * 1.5f) / 2.0f + 0.5f;
    m_tetra->m_material->setUniform("u_color", vec4(0.0f, osc, 0.8f, 1.0f));
    m_tetra->setRotation({ time * 100.0f, 0.0f, 0.0f });

    if (m_state.inGroup("countdown")) {
        m_tetra->m_material->setUniform("u_color", vec4(osc, 0.0f, 0.3f, 1.0f));

        m_camera->m_enableFreeCam = false;
        m_camera->m_autoRotate = false;
        m_camera->m_fov = 65;
        Anim::lerp(m_camera->m_camRadius, 2.5f, 0.05f);
    }

    if (m_state.is("PLAYING")) {
        const float beatMatch = (145.0f / 60.0f) * time * 2.0f * M_PI; 
        m_tetra->setScale(sin(beatMatch) * 0.3f + 0.8f);
        m_tetra->setRotation({ 0.0f, beatMatch * 180.0f/M_PI, 0.0f });
    }

    // @TODO move to State::onUpdate() / State::onInit() as anonymous func?
    if (m_state.is("Finish")) {
        m_layers.get("overlay")->m_active = (int) time % 2 == 0;
        m_camera->m_autoRotate = true;
    }

    /* Player controls */
    if (!m_camera->m_enableFreeCam) {
        m_camera->m_lookAtPos = m_player->localToWorld({ 0, 1.2f, 0 });
        m_camera->setPosition(m_player->localToWorld({ 0, 1.5f, -3.4f }));
    
        if (Keyboard::w) 
            m_player->applyForce(m_player->localToWorld({ 0, 0, 100.0f }), m_player->localToWorld({ 0, 0, 0 }));
        if (Keyboard::s) 
            m_player->applyForce(m_player->localToWorld({ 0, 0, -100.0f }), m_player->localToWorld({ 0, 0, 0 }));
        if (Keyboard::a) 
            m_player->applyTorque({ 0, 25.0f, 0 });
        if (Keyboard::d) 
            m_player->applyTorque({ 0, -25.0f, 0 });
    }

    m_camera->update(time); /* Camera must be updated here to prevent lag */
    m_state.update(time, dt); // @TODO move to base Scene update
    m_audio->updateListener(m_camera->getPosition(), m_camera->getForward(), m_camera->getUp());
}

void MyScene::destroy() {}
