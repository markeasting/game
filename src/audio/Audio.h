#pragma once

#include "common.h"
#include "AL/alure2.h"

struct AudioSource {
    bool isStream = false;
    bool isBuffer = false;

    alure::Source source;

    AudioSource(alure::Source source) : source(source) {}

    inline virtual void play() = 0;
    inline void stop() {
        source.stop();
    };

    // @TODO chain pattern
    void setPosition(vec3 pos) {
        source.set3DSpatialize(alure::Spatialize::On);
        source.setPosition(glm::value_ptr(pos));
        // source.setVelocity(pos);
    }
};

struct BufferAudioSource : public AudioSource {
    bool isBuffer = true;

    alure::Buffer buffer;

    BufferAudioSource(alure::Source source)
        : AudioSource(source)
    {}

    BufferAudioSource(alure::Source source, alure::Buffer)
        : AudioSource(source), buffer(buffer)
    {}

    inline void play() override {
        source.play(buffer);
    }
};

struct StreamAudioSource : public AudioSource {
    bool isStream = true;

    Ref<alure::Decoder> decoder = nullptr;
    
    StreamAudioSource(alure::Source source, Ref<alure::Decoder> decoder) 
        : AudioSource(source), decoder(decoder)
    {}

    inline void play() override {
        source.play(decoder, 12000, 4);
    }
};

class Audio {
public:

    Audio();

    Ref<BufferAudioSource> add(const char* handle, const char* filename);
    Ref<StreamAudioSource> stream(const char* handle, const char* filename);

    alure::Source get(const char* handle);

    void play(const char* handle);
    void stop(const char* handle);

    // void fadeIn(const char* handle, loat time_ms = 1000);
    void fadeOut(const char* handle, int time_ms = 1000);

    void update();
    void updateListener(vec3 pos, vec3 dir, vec3 up);

    void destroy();

private:

    std::unordered_map<const char*, alure::Buffer> m_buffers;
    std::unordered_map<const char*, Ref<AudioSource>> m_sources;

    alure::DeviceManager m_devMgr;
    alure::Device m_device;
    alure::Context m_ctx;

};
