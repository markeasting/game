#pragma once

#include <iostream>
#include <unordered_map>

#include "AL/alure2.h"
#include "util/log.h"

class Audio {
public:

    // Audio();
    Audio(alure::Context ctx);

    // static void init(); // Doesn't work :(

    void load(const char* handle, const char* filename);
    alure::Source createSource(const char* handle, const char* filename = "");
    // alure::Source createSource3D(const char* handle, const char* filename = "");

    void play(const char* handle);
    void stop(const char* handle);

    void destroy();

    inline void update() { m_ctx.update(); };

private:

    std::unordered_map<const char*, alure::Source> m_sources;
    std::unordered_map<const char*, alure::Buffer> m_buffers;

    alure::Device m_device;
    alure::Context m_ctx;

};
