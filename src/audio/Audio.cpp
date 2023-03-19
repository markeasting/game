
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

void Audio::load(const char* handle, const char* filename) {
    
    if(m_buffers.find(handle) != m_buffers.end()) {
        Log("Audio buffer already exists!", handle);
        return;
    }

    Log("Loading", filename);

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
    // source.set3DSpatialize(alure::Spatialize::On);

    m_sources[handle] = source;

    return source;
}

alure::Source Audio::get(const char* handle) {
    assert(m_sources.find(handle) != m_sources.end()); // Handle not found

    return m_sources[handle];
}

void Audio::play(const char* handle) {

    assert(m_sources.find(handle) != m_sources.end()); // Handle not found
    assert(m_buffers.find(handle) != m_buffers.end()); // Handle not found

    auto buff = m_buffers[handle];

    std::cout<< "Playing "<< handle <<" ("
        << alure::GetSampleTypeName(buff.getSampleType())<<", "
        << alure::GetChannelConfigName(buff.getChannelConfig())<<", "
        << buff.getFrequency()<<"hz)" <<std::endl;

    m_sources[handle].play(buff);

}

void Audio::stop(const char* handle) {

    assert(m_sources.find(handle) != m_sources.end()); // Handle not found
    assert(m_buffers.find(handle) != m_buffers.end()); // Handle not found

    m_sources[handle].stop();

}

// @TODO this is currently never called
void Audio::destroy() {

    for (auto source : m_sources) {
        source.second.destroy();
    }

    for (auto buff : m_buffers) {
        m_ctx.removeBuffer(buff.second);
    }

    alure::Context::MakeCurrent(nullptr);

    auto dev = m_ctx.getDevice();
    
    m_ctx.destroy();

    dev.close();

}
