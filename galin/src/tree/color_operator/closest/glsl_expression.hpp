#pragma once

#include "closest_color.hpp"

#include <string>

inline
std::string glsl_expression(
    ClosestColor,
    const std::string& s0,
    const std::string& s1)
{
    return "closest_color(" + s0 + ", " + s1 + ")";
}
