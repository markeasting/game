#pragma once

#include <mutex>

template <typename T>
struct Singleton {

    /** Singletons should not be cloneable */
    // Singleton(Singleton &other) = delete; // Breaks the `new T()` call...
    
    /** Singletons should not be assignable */
    void operator=(const Singleton &) = delete;

    /** 
     * Get an instance (thread safe).
     * 
     * https://refactoring.guru/design-patterns/singleton/cpp/example#example-1
     */
    static T* instance() {
        std::lock_guard<std::mutex> lock(m_mutex);

        if (m_instance == nullptr) { 
            m_instance = new T(); 
        }

        return m_instance;
    };

protected:
    inline static T* m_instance = nullptr;
    inline static std::mutex m_mutex;
};
