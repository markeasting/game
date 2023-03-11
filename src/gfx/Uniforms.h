#pragma once

#include "common.h"
#include <string>

class IUniform {
public:

    std::string m_name;
    int m_location = -1;
    int m_value;

    IUniform(const std::string& name)
        : m_name(name) {};

    virtual ~IUniform() = default;

    virtual void bind() {};

protected:
    bool m_needsUpdate = true;
};

template <typename T>
class Uniform : public IUniform {
public:

    Uniform(const std::string& name, const T &value = {}) 
        : IUniform(name), m_value(value) {}

    virtual void bind();

    virtual void set(T value) { 
        m_needsUpdate = true; 
        m_value = value;
    }

private:
    T m_value;

};
