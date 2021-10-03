#pragma once

#include "colored.hpp"

#include <string>

template<typename... C, typename... Args>
std::string distance_glsl_expression(
    const Colored<C...>& c,
    Args&&... args)
{
    return distance_glsl_expression(c.any, std::forward<Args>(args)...);
}
