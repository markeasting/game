
#include "scenes/Test/MyScene.h"
#include "physics/Collider.h"
#include "scene/SceneManager.h"
#include "util/Anim.h"
#include "gfx/CubeMapTexture.h"

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

    auto lightDirection = uniform("u_lightDirection", vec3(0.5f, 0.0f, 2.0f));

    // Material phongMaterial = Material("Phong", { lightDirection });
    Material colorMaterial = Material("Color", {
        uniform("u_color", vec4(0.0f, 0.0f, 0.8f, 1.0f)),
    });

    auto cube = ref<Mesh>(BoxGeometry(0.5f), colorMaterial);
        cube->setPosition({ 2.0f, 0.5f, 0.0f });
        m_world->add(cube);

    m_tetra = ref<Mesh>(TetrahedronGeometry(1.0f), colorMaterial);
        m_tetra->setPosition({ 0.0f, 1.0f, 0.0f });
        m_world->add(m_tetra);
        m_tetra->add(cube);

    Material skyMaterial = Material("SkyBox", {
        uniform("u_color", vec4(0.0f, 0.0f, 0.8f, 1.0f)),
    });
    Ref<CubeMapTexture> skyTexture = ref<CubeMapTexture>();
    skyTexture->loadCubemap({
        "assets/texture/skybox/right.jpg",
        "assets/texture/skybox/left.jpg",
        "assets/texture/skybox/bottom.jpg",
        "assets/texture/skybox/top.jpg",
        "assets/texture/skybox/front.jpg",
        "assets/texture/skybox/back.jpg"
    });
    skyMaterial.assignTexture(skyTexture, "texture1");
    m_skybox = ref<Mesh>(BoxGeometry(1.0f, true), skyMaterial);
        m_world->add(m_skybox);

    /* Physics world */

    auto car = ref<Geometry>(obj::loadModelFromFile("assets/objects/car/car.obj"));
    auto car_collider = ref<Geometry>(obj::loadModelFromFile("assets/objects/car/car_collider.obj"));
    auto colliderSize = vec3(1.42f, 0.95f, 3.0f);

    // m_player = ref<RigidBody>(
    //         ref<MeshCollider>(car_collider),
    //         ref<Mesh>(car, Material("Phong", { lightDirection }))
    //     );
    //     m_player->setBox(colliderSize);
    //     m_player->setPosition({ 4.0f, 2.0f, -3.0f });
    //     // m_player->makeStatic();
    //     m_world->add(m_player);
    //     m_phys.add(m_player);

    m_player = ref<Car>(m_phys);
    m_player->addTo(m_world);

    // auto opponent = ref<RigidBody>(
    //         ref<MeshCollider>(car_collider),
    //         ref<Mesh>(car, Material("Phong", { lightDirection, uniform("ambient", vec3(0, 0, 0.2)), uniform("diffuseAlbedo", vec3(0, 0, 0.7)) }))
    //     );
    //     opponent->setBox(colliderSize, 200.0f);
    //     opponent->setPosition({ -4.0f, 2.0f, -3.0f });
    //     m_world->add(opponent);
    //     m_phys.add(opponent);

    for (size_t i = 0; i < 2; i++) {
        auto opponent = ref<Car>(m_phys);
        opponent->m_body->setPosition({ 6.0f + 2.0f * i, 2.0f, -3.0f });
        opponent->addTo(m_world);

        m_opponents.push_back(opponent);
    }

    // auto sphere = ref<RigidBody>(
    //         ref<Mesh>(SphereGeometry(1.0f, 3), colorMaterial)
    //     );
    //     sphere->setPosition({ -4.0f, 2.0f, 0.0f });
    //     sphere->setBox(vec3(1.0f), 1.0f);
    //     sphere->canSleep = false;
    //     sphere->restitution = 1.0f;
    //     m_world->add(sphere);
    //     m_phys.add(sphere);

    /* Dominos */
    // for (size_t i = 0; i < 50; i++) {
    //     auto box = ref<RigidBody>(
    //             ref<BoxCollider>(vec3(2.5f, 5.0f, 0.5f)), 
    //             ref<Mesh>(BoxGeometry(2.5f, 5.0f, 0.5f), colorMaterial)
    //         );
    //         box->setPosition({ -4.0f, 2.5f, 0.0f + i * -2.5f });
    //         box->setBox(vec3(2.5f, 5.0f, 0.5f), 2.0f);
    //         m_world->add(box);
    //         m_phys.add(box);
    // }

    /* Tall stack */
    // for (size_t i = 0; i < 4; i++) {
    //     auto box = ref<RigidBody>(
    //             ref<BoxCollider>(1.0f), 
    //             ref<Mesh>(BoxGeometry(1.0f), colorMaterial)
    //         );
    //         box->setPosition({ 4.0f, 0.5f + i * 1.01f, 0.0f });
    //         box->setBox(vec3(1.0f), 133.0f);
    //         box->canSleep = false;
    //         m_world->add(box);
    //         m_phys.add(box);
    // }

    Material floorMaterial = Material("Basic.vert", "BasicTextured.frag");
    floorMaterial.assignTexture("assets/texture/asphalt.jpg", "texture1");

    auto floorMesh = ref<Mesh>(PlaneGeometry(200.0f, true), floorMaterial);
    auto floor = ref<RigidBody>(floorMesh);
        floor->name = "Floor1";
        floor->setPosition({ 0, 0, -75.0f });
        floor->makeStatic();
        m_world->add(floor);
        m_phys.add(floor);

    auto floorMesh2 = ref<Mesh>(PlaneGeometry(200.0f, true), floorMaterial);
    auto floor2 = ref<RigidBody>(floorMesh2);
        floor2->name = "Floor2";
        floor2->setPosition({ 0, 5.2f, 124.0f });
        floor2->setRotation(QuatFromTwoVectors({0, 1.0f, 0}, {0, 0.95f, -0.05f}));
        floor2->makeStatic();
        m_world->add(floor2);
        m_phys.add(floor2);

    // auto floor = ref<RigidBody>(
    //         ref<PlaneCollider>(vec2(200.0f, 200.0f)),
    //         ref<Mesh>(PlaneGeometry(200.0f, true), floorMaterial)
    //     );
    //     floor->name = "Floor1";
    //     floor->setPosition({ 0, 0, -75.0f });
    //     floor->makeStatic();
    //     m_world->add(floor);
    //     m_phys.add(floor);

    // auto floor2 = ref<RigidBody>(
    //         ref<PlaneCollider>(vec2(200.0f, 200.0f)),
    //         ref<Mesh>(PlaneGeometry(200.0f, true), floorMaterial)
    //     );
    //     floor2->name = "Floor2";
    //     floor2->setPosition({ 0, 5.2f, 124.0f });
    //     floor2->setRotation(QuatFromTwoVectors({0, 1.0f, 0}, {0, 0.95f, -0.05f}));
    //     floor2->makeStatic();
    //     m_world->add(floor2);
    //     m_phys.add(floor2);

    auto track = ref<Geometry>(obj::loadModelFromFile("assets/objects/track.obj"));
    auto trackMesh = ref<Mesh>(track, floorMaterial);
    // m_world->add(trackMesh);
    // m_phys.addStaticMesh(trackMesh);

    auto trackBody = ref<RigidBody>(trackMesh);
        trackBody->name = "TrackBody";
        trackBody->makeStatic();
        trackBody->canCollide = false;
        m_world->add(trackBody);
        m_phys.add(trackBody);

    m_phys.init();
    for (auto const& mesh : m_phys.m_debugMeshes) 
        m_world->add(mesh);

}

/**
 * @TODO these listeners are currently bound for the entire lifetime of the game
 * Should encapsulate each scene to have their own event bus
 */
void MyScene::bindEvents() {

    Events::on(Events::KEYDOWN, [&](SDL_KeyCode key) {
        switch (key) {
            case SDLK_i:
                m_player->m_body->applyForce(vec3(0, 25000.0f, 0), m_player->m_body->localToWorld({ 0, 0, 0 }));
                break;
            case SDLK_l:
                m_player->m_body->applyForce(vec3(0, 10000.0f, 0));
                break;
        }
    });

    Events::on(Events::KEYUP, [&](SDL_KeyCode key) {
        switch (key) {
            case SDLK_SPACE:
                // m_state.next();
                m_phys.update(0.016f, [this](float h){});
            break;
        }
    });

    Events::on(Events::MOUSEUP, [&]() {
        // m_camera->m_autoRotate = !m_camera->m_autoRotate;
        m_camera->m_enableFreeCam = !m_camera->m_enableFreeCam;
        SDL_SetRelativeMouseMode(m_camera->m_enableFreeCam ? SDL_TRUE : SDL_FALSE);
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

    if (time > 1.0f)
        SDL_SetRelativeMouseMode(SDL_TRUE);
    
    m_camera->update(time);
    
    // if (Keyboard::space) {
        m_phys.update(dt, [this](float h){
            m_player->update(h);

            for (auto& o : m_opponents)
                o->update(h);
        });
    
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
        // m_layers.get("overlay")->m_active = (int) time % 2 == 0;
        m_camera->m_autoRotate = true;
    }

    /* Player controls */
    if (!m_camera->m_enableFreeCam) {
        // m_camera->m_lookAtPos = m_player->m_body->localToWorld(m_player->m_camLookPos);
        // m_camera->setPosition(m_player->m_body->localToWorld(m_player->m_camPos));
        m_camera->m_lookAtPos = m_player->m_camLookPos;
        m_camera->setPosition(m_player->m_camPos);
    
        if (Keyboard::w) {
            Anim::lerp(m_player->m_throttle, 1.0f, 0.15f);
            // m_player->applyThrottle(1.0f);
        } else if (Keyboard::s) {
            Anim::lerp(m_player->m_throttle, -1.0f, 0.15f);
            // m_player->applyThrottle(-1.0f);
        } else {
            Anim::lerp(m_player->m_throttle, 0.0f, 0.15f);
        }

        if (Keyboard::a) {
            Anim::lerp(m_player->m_steering, -1.0f, (clamp(glm::length(m_player->m_body->vel) / 50.0f, 0.09f, 0.15f)));
        } else if (Keyboard::d) {
            Anim::lerp(m_player->m_steering, 1.0f, (clamp(glm::length(m_player->m_body->vel) / 50.0f, 0.09f, 0.15f)));
        } else {
            Anim::lerp(m_player->m_steering, 0.0f, (clamp(glm::length(m_player->m_body->vel) / 50.0f, 0.09f, 0.15f)));
        }

        m_player->m_handbrake = Keyboard::b;

        // if (Keyboard::a) 
        //     m_player->applySteering(-1.0f);
        // if (Keyboard::d) 
        //     m_player->applySteering(1.0f);
    }

    m_state.update(time, dt); // @TODO move to base Scene update
    m_audio->updateListener(m_camera->getPosition(), m_camera->getForward(), m_camera->getUp());
}

void MyScene::destroy() {}
