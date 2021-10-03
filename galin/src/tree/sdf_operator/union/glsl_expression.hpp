#pragma once

#include "union.hpp"

#include <string>

inline
std::string glsl_expression(
    SDF_Union,
    const std::string& s0,
    const std::string& s1)
{
    return "min(" + s0 + ", " + s1 + ")";
}
