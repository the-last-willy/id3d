#pragma once

#include "tree2/glsl_expression.hpp"
#include "colored_distance.hpp"

#include <sstream>

inline
std::string glsl_expression(const ColoredDistance& cd) {
    auto ss = std::stringstream();
    ss << "ColoredDistance("
    << cd.distance << ", "
    << glsl_expression(cd.color) << ")";
    return ss.str();
}
