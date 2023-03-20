#pragma once

#include <unordered_map>

#include "scene/Layer.h"

class LayerManager {
public:
    LayerManager() = default;

    /**
     * Called by ScenManager before Scene.init()
     */
    void initLayers() {
        for (auto& pair : m_layers) {
            pair.second->init();
        }
    }

    Ref<Layer> add(const char* name, Ref<Layer> layer) {
        assert(layer != nullptr);
        m_layers[name] = layer;

        return layer;
    }

    Ref<Layer> get(const char* name) {
        assert(m_layers[name] != nullptr);
        return m_layers[name];
    }

    std::unordered_map<const char*, Ref<Layer>>& all() {
        return m_layers;
    }
    
private:
    std::unordered_map<const char*, Ref<Layer>> m_layers = {};

};
