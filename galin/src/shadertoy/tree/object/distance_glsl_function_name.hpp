#pragma once

#include "object.hpp"

#include <string>

template<typename O>
std::string distance_glsl_function_name(const Object<O>& o) {
    return o.name + "_distance";
}
