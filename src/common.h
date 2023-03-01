#pragma once

// @TODO add util/log.h as standard

/* C / C++ */
#define _USE_MATH_DEFINES
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <memory>

/* Includes */
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_INTRINSICS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/norm.hpp>

/* Application */
#define LOG_LEVEL DEBUG

/* Types */
template<typename T>
using Ref = std::shared_ptr<T>;

// template<typename T, typename... _Args>
// Ref<T> newRef(_Args&&... __args)
// {
//     std::make_shared<T>(__args);
// }
