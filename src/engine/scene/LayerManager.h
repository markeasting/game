#pragma once

#include <unordered_map>

#include "engine/scene/Layer.h"

class LayerManager {
public:
    LayerManager() = default;

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
