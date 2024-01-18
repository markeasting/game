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
 * auto container = Container::instance();
 *
 * container->transient<Bar>(BarConfig({ .debug =  true }));
 * container->singleton<Foo, Bar>();
 * container->singleton<MyClass, Foo>();
 */
class Container {
public:

    /** Singletons should not be cloneable */
    Container(Container &other) = delete;
    
    /** Singletons should not be assignable */
    void operator=(const Container &) = delete;
    
    /** 
     * Get a container instance. 
     * https://refactoring.guru/design-patterns/singleton/cpp/example#example-1
     */
    static Container* instance();

    /**
     * Registers a class as a singleton / shared service.
     */
    template <typename T, typename... Dependencies, typename... Args>
    void singleton(Args&&... args) {

        /* Create a constructor/factory function */
        m_factories[typeid(T)] = [this, args...] {
            const auto instance = std::make_shared<T>(
                this->get<Dependencies>()..., 
                args...
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
        m_factories[typeid(T)] = [this, args...] {
            return std::make_shared<T>(
                this->get<Dependencies>()..., 
                args...
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
    std::shared_ptr<T> get() {

        /* Check if the service has already been constructed */
        auto instanceIt = m_services.find(typeid(T));
        if (instanceIt != m_services.end()) {
            return std::static_pointer_cast<T>(instanceIt->second);
        }

        /* No instance found, construct the service and store it */
        /* @TODO for transients, skip storing the instance */
        auto serviceIt = m_factories.find(typeid(T));
        if (serviceIt != m_factories.end()) {

            auto instance = serviceIt->second();

            return std::static_pointer_cast<T>(instance);
        }

        throw std::runtime_error(
            std::string("Service not registered: ") + typeid(T).name()
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

private:

    inline static Container* _instance = nullptr;
    inline static std::mutex _mutex;

    Container() {}
    ~Container() {}
};
