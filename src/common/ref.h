#pragma once

#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using WeakRef = std::weak_ptr<T>;

template<typename T, typename ... Args>
constexpr Ref<T> ref(Args&& ... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

// template<typename T>
// using Unique = std::unique_ptr<T>;
