#pragma once

#include <unordered_map>
#include <memory>
#include <typeindex>
#include <functional>
#include <stdexcept>

/**
 * Application container - Dependency injector / service locator
 *
 * @example
 * container.add<Bar>(BarConfig({ .debug =  true }));
 * container.add<Foo, Bar>();
 * container.add<MyClass, Foo>();
 */
class Container {
public:

    /**
     * Registers a class as a service.
     */
    template <typename T, typename... Dependencies, typename... Args>
    void add(Args&&... args) {

        /* Create a constructor/factory function */
        services[std::type_index(typeid(T))] = [this, args...] {
            return std::make_shared<T>(get<Dependencies>()..., args...);
        };
    }

    /**
     * Retrieves a service from the container, and resolves it's dependencies. 
     *
     * @tparam T the service type
     * @return std::shared_ptr<T> shared instance
     */
    template <typename T>
    std::shared_ptr<T> get() {

        /* Check if the service has already been constructed */
        auto instanceIt = instances.find(typeid(T));
        if (instanceIt != instances.end()) {
            return std::static_pointer_cast<T>(instanceIt->second);
        }

        /* Construct the service instance and store it */
        auto serviceIt = services.find(typeid(T));
        if (serviceIt != services.end()) {
            
            auto instance = serviceIt->second();
            instances[typeid(T)] = instance;

            return std::static_pointer_cast<T>(instance);
        }

        throw std::runtime_error(
            std::string("Service not registered: ") + typeid(T).name()
        );
    }

private:

    /* Map of service factories. */
    std::unordered_map<
        std::type_index, 
        std::function<std::shared_ptr<void>()>
    > services;
    
    /* Map of constructed service instances. */
    std::unordered_map<
        std::type_index, 
        std::shared_ptr<void>
    > instances;

    // template <typename T, typename First, typename... Rest>
    // std::shared_ptr<T> resolve() {
    //     /* Resolve the first dependency and recursively resolve the rest */
    //     return std::make_shared<T>(get<First>(), get<Rest>()...);
    // }
};
