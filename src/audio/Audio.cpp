
#include "audio/Audio.h"

// std::unordered_map<const char*, alure::Source> Audio::m_sources;
// std::unordered_map<const char*, alure::Buffer> Audio::m_buffers;

// alure::Device Audio::m_device;
// alure::Context Audio::m_ctx;

Audio::Audio(alure::Context ctx) : m_ctx(ctx) {}

void Audio::load(const char* handle, const char* filename) {
    
    if(m_buffers.find(handle) != m_buffers.end()) {
        Log("Audio buffer already exists!", handle);
        return;
    }

    Log(handle);
    Log(filename);

    alure::Buffer buffer = m_ctx.getBuffer(filename);
    m_buffers[handle] = buffer;
}

alure::Source Audio::createSource(const char* handle, const char* filename) {

    if(m_buffers.find(handle) != m_buffers.end()) {
        Log("Audio source already exists!", handle);
        return m_sources[handle];
    }

    if (filename) {
        this->load(handle, filename);
    }

    alure::Source source = m_ctx.createSource();

    m_sources[handle] = source;

    return source;
}

void Audio::play(const char* handle) {

    assert(m_sources.find(handle) != m_sources.end());
    assert(m_buffers.find(handle) != m_buffers.end());

    auto buff = m_buffers[handle];

    std::cout<< "Playing "<< handle <<" ("
        << alure::GetSampleTypeName(buff.getSampleType())<<", "
        << alure::GetChannelConfigName(buff.getChannelConfig())<<", "
        << buff.getFrequency()<<"hz)" <<std::endl;

    m_sources[handle].play(buff);

}

void Audio::stop(const char* handle) {

    assert(m_sources.find(handle) != m_sources.end());
    assert(m_buffers.find(handle) != m_buffers.end());

    m_sources[handle].stop();

}

// @TODO this is currently never called
void Audio::destroy() {

    // for (auto source : m_sources) {
    //     source.second.destroy();
    // }

    // for (auto buff : m_buffers) {
    //     m_ctx.removeBuffer(buff.second);
    // }

    // alure::Context::MakeCurrent(nullptr);

    // auto dev = m_ctx.getDevice();
    
    // m_ctx.destroy();

    // dev.close();

}
