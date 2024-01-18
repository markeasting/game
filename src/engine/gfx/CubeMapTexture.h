#pragma once

#include "engine/gfx/Texture.h"

#include <vector>
#include <string>

class CubeMapTexture : public Texture {
public: 

    CubeMapTexture() = default;
    CubeMapTexture(const std::vector<std::string>& faces);

    void loadCubemap(const std::vector<std::string>& faces);

    void bind() const;

};
