#pragma once

#include "object.hpp"

template<typename... O>
std::string glsl_distance_function_name(const Object<O...>& o) {
    auto ss = std::stringstream();
    ss << o.name() << "_distance";
    return ss.str();
}
