
#include "scenes/Test/MainUI.h"
#include "geom/index.h"

void MainUI::init() {

    Material textureMaterial = Material("Basic.vert", "BasicTextured.frag");
    textureMaterial.assignTexture("assets/texture/uv_test.jpg", "texture1");
    textureMaterial.assignTexture("assets/texture/checker_purple.png", "texture2");

    auto overlay = ref<Mesh>(PlaneGeometry(2.0f));
    
    overlay->setMaterial(textureMaterial);
    overlay->setPosition({ 0, -0.5f, 0 });
    overlay->setScale({ 0.3f, 0.3f, 0 });
    
    overlay->m_useProjectionMatrix = false;

    this->add(overlay);

}
