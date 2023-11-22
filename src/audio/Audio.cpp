
#include "audio/Audio.h"

// Audio& Audio::Instance() {
//     static Audio instance;
//     return instance;
// }

Audio::Audio() {
    m_devMgr = alure::DeviceManager::getInstance();

    if(!m_device) m_device = m_devMgr.openPlayback();
    
    m_ctx = m_device.createContext();
    alure::Context::MakeCurrent(m_ctx);

}

Ref<BufferAudioSource> Audio::add(const char* handle, const char* filename) {

    Log(filename, "Loading audio buffer: ");

    alure::Buffer buffer = m_ctx.getBuffer(filename);
    alure::Source source = m_ctx.createSource();
    // source.set3DSpatialize(alure::Spatialize::On);

    auto newSource = ref<BufferAudioSource>(source);
    newSource->buffer = buffer;

    m_sources[handle] = newSource;
    m_buffers[handle] = buffer;

    return newSource;
}

Ref<StreamAudioSource> Audio::stream(const char* handle, const char* filename) {

    Log(filename, "Streaming audio: ");

    alure::Source source = m_ctx.createSource();
    // source.set3DSpatialize(alure::Spatialize::On);

    auto newSource = ref<StreamAudioSource>(source, m_ctx.createDecoder(filename));

    m_sources[handle] = newSource;

    return newSource;
}

alure::Source Audio::get(const char* handle) {
    assert(m_sources.find(handle) != m_sources.end());

    return m_sources[handle]->source;
}

void Audio::play(const char* handle) {

    assert(m_sources.find(handle) != m_sources.end());

    // auto buff = m_buffers[handle];
    // std::cout<< "Playing "<< handle <<" ("
    //     << alure::GetSampleTypeName(buff.getSampleType())<<", "
    //     << alure::GetChannelConfigName(buff.getChannelConfig())<<", "
    //     << buff.getFrequency()<<"hz)" <<std::endl;

    m_sources[handle]->play();

}

void Audio::stop(const char* handle) {
    assert(m_sources.find(handle) != m_sources.end());
    m_sources[handle]->stop();
}

void Audio::fadeOut(const char* handle, int time_ms) {
    assert(m_sources.find(handle) != m_sources.end());
    m_sources[handle]->source.fadeOutToStop(0.0f, std::chrono::milliseconds(time_ms));
}

void Audio::update() {
    m_ctx.update();
}

void Audio::updateListener(vec3 pos, vec3 dir, vec3 up) {
    auto listener = m_ctx.getListener();

    listener.setPosition(glm::value_ptr(pos));
    listener.setOrientation(glm::value_ptr(dir), glm::value_ptr(up));
}


// @TODO this is currently never called
void Audio::destroy() {

    for (auto& [key, audioSource] : m_sources) {
        audioSource->source.destroy();

        // @TODO clean up buffers -- isn't this also done by deleting the context?
        // if (audioSource->isBuffer) {
        //     auto buff = dynamic_cast<BufferAudioSource>(audioSource);
        // }
    }

    alure::Context::MakeCurrent(nullptr);

    m_ctx.destroy();
    m_device.close();

}
