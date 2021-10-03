#pragma once

#include "colored.hpp"
#include "distance_glsl_expression.hpp"

#include "tree/glsl_expression.hpp"

#include <string>

template<typename... C, typename... Args>
std::string colored_distance_glsl_expression(
    const Colored<C...>& c,
    Args&&... args)
{
    return "ColoredDistance("
    + distance_glsl_expression(c, std::forward<Args>(args)...) + ", "
    + glsl_expression(c.color) + ")";
}
