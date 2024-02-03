#pragma once

#include "engine/common/ref.h"
#include <string>

class IUniform {
public:

    std::string m_name;
    int m_location = -1;
    int m_value = -1;

    IUniform(const std::string& name)
        : m_name(name) {};

    virtual ~IUniform() = default;

    virtual void set() {};
    virtual void bind() {};

protected:
    bool m_needsUpdate = true;
};

template <typename T>
class Uniform : public IUniform {
public:

    T m_value;

    Uniform(const std::string& name, T value = {}) 
        : IUniform(name), m_value(value) {}

    void set(T value) { 
        m_needsUpdate = true; 
        m_value = value;
    }
    
    void bind() override {};

};

template <typename T>
Ref<Uniform<T>> uniform(const std::string& name, T value = {}) {
    return ref<Uniform<T>>(name, value);
}
