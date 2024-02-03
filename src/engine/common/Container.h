#pragma once

#include "engine/util/Singleton.h"

#include <unordered_map>
#include <memory>
#include <typeindex>
#include <functional>
#include <stdexcept>

/**
 * Application container - Dependency injector / service locator
 *
 * @example
 * auto container = Container::instance();
 *
 * container->transient<Bar>(BarConfig({ .debug =  true }));
 * container->singleton<Foo, Bar>();
 * container->singleton<MyClass, Foo>();
 */
class Container : public Singleton<Container> {
public:

    // /** Singletons should not be cloneable */
    // Container(Container &other) = delete;
    
    // /** Singletons should not be assignable */
    // void operator=(const Container &) = delete;

    /**
     * Registers a class as a singleton / shared service.
     */
    template <typename T, typename... Dependencies, typename... Args>
    void singleton(Args&&... args) {

        /* Create a constructor/factory function */
        m_factories[typeid(T)] = [this, &args...] {
            const auto instance = std::make_shared<T>(
                this->resolve<Dependencies>()..., 
                std::forward<Args>(args)...
            );

            m_services[typeid(T)] = instance; // Store shared instance

            return instance;
        };
    }

    /**
     * Registers a class as a transient service.
     */
    template <typename T, typename... Dependencies, typename... Args>
    void transient(Args&&... args) {

        /* Create a constructor/factory function */
        m_factories[typeid(T)] = [this, &args...] {
            return std::make_shared<T>(
                this->resolve<Dependencies>()..., 
                std::forward<Args>(args)...
            );
        };
    }

    /**
     * Retrieves a service from the container, and resolves it's dependencies. 
     *
     * @tparam T the service type
     * @return std::shared_ptr<T> shared instance
     */
    template <typename T>
    std::shared_ptr<T> resolve() {

        /* Check if the service has already been constructed */
        auto instanceIt = m_services.find(typeid(T));
        if (instanceIt != m_services.end()) {
            return std::static_pointer_cast<T>(instanceIt->second);
        }

        /* No instance found, construct the service */
        auto serviceIt = m_factories.find(typeid(T));
        if (serviceIt != m_factories.end()) {

            auto instance = serviceIt->second();

            return std::static_pointer_cast<T>(instance);
        }

        throw std::runtime_error(
            std::string(typeid(T).name()) + " is not a registered service. Did you forget to register this service or did you forget to configure a bundle?"
        );
    }

protected:

    /* Map of service factories. */
    std::unordered_map<
        std::type_index, 
        std::function<std::shared_ptr<void>()>
    > m_factories;
    
    /* Map of constructed service instances. */
    std::unordered_map<
        std::type_index, 
        std::shared_ptr<void>
    > m_services;

// private:
//     Container() {}
//     ~Container() {}
};
