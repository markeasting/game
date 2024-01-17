#pragma once

#ifndef NDEBUG
    #define DEBUG_MODE
#endif

#define _USE_MATH_DEFINES
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <array>
#include <functional>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <SDL.h>
#include <glad/glad.h>

#define GLM_FORCE_CXX17
#define GLM_FORCE_INTRINSICS // @TODO test performance
// #define GLM_FORCE_INLINE // @TODO test performance
#include "glm/glm.hpp"
#include "glm/gtx/norm.hpp"
#include "glm/gtc/type_ptr.hpp"
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtx/vector_angle.hpp>
// #include <glm/gtx/norm.hpp>
using namespace glm;

#include "common/ref.h"
#include "common/log.h"
