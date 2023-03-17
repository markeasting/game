
#include "scenes/MyScene.h"

MyScene::MyScene() {
    // m_camera = ref<Camera>();
}

void MyScene::create() {
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
    this->add(floor);

    auto sphere = ref<Mesh>(SphereGeometry(1.0f), phongMaterial);
    sphere->setPosition({ 0.0f, 0.5f, 3.0f });
    this->add(sphere);

    auto box = ref<Mesh>(BoxGeometry(0.5f), colorMaterial);
    box->setPosition({ 2.0f, 0.5f, 0.0f });
    this->add(box);

    m_tetra = ref<Mesh>(TetrahedronGeometry(1.0f), colorMaterial);
    m_tetra->setPosition({ 0.0f, 1.0f, 0.0f });
    this->add(m_tetra);

    m_tetra->add(box);

    auto overlay = ref<Mesh>(PlaneGeometry(2.0f));
    Material textureMaterial = Material("Basic.vert", "BasicTextured.frag");
    textureMaterial.assignTexture("assets/texture/uv_test.jpg", "texture1");
    textureMaterial.assignTexture("assets/texture/checker_purple.png", "texture2");
    overlay->setMaterial(textureMaterial);
    overlay->setPosition({ 0, -0.5f, 0 });
    overlay->setScale({ 0.3f, 0.3f, 0 });
    overlay->m_useProjectionMatrix = false;
    this->add(overlay);

}

void MyScene::update(float time, float deltaTime) {
    // m_camera->update(time, m_keyboard);

    m_tetra->setRotation({ time * 100.0f, 0.0f, 0.0f });
    
    float oscillator = sin(time * 1.5) / 2.0f + 0.5f;
    m_tetra->m_material->setUniform("u_color", glm::vec4(0.0f, oscillator, 0.8f, 1.0f));
}


void MyScene::destroy() {}

