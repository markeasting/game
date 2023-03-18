#pragma once

#ifdef NDEBUG
    #define LOG_LEVEL 0
#else
    #define LOG_LEVEL 2
#endif

/* C / C++ */
#define _USE_MATH_DEFINES
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <memory>

/* Libraries / includes */
#include <SDL.h>
#include <glad/glad.h>

#define GLM_FORCE_INTRINSICS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>

/* Application */
#include "util/log.h"
#include "event/Event.h"
#include "event/EventEmitter.h"
#include "input/KeyboardHandler.h"

/* Types */
template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
using Unique = std::unique_ptr<T>;

template<typename T, typename... _Args>
Ref<T> ref(_Args&&... args) {
    return std::make_shared<T>(args...);
}

// template<typename T>
// Ref<T> ref(T&& obj) {
//     return std::make_shared<T>(obj);
// }

// template<typename T>
// Ref<T> ref(const T& obj) {
//     return std::make_shared<T>(obj);
// }

// template<typename T>
// Unique<T> unique(T&& obj) {
//     return std::make_unique<T>(obj);
// }
