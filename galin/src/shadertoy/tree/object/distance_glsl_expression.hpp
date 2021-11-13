#pragma once

#include "distance_glsl_function_name.hpp"
#include "object.hpp"

#include <string>

template<typename O>
std::string distance_glsl_expression(
    const Object<O>& o,
    const std::string& argument)
{
    return distance_glsl_function_name(o) + "("
    + argument + ")";
}
