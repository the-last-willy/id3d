#pragma once

#include "colored_distance_glsl_function_name.hpp"
#include "object.hpp"

#include <string>

template<typename O>
std::string colored_distance_glsl_expression(
    const Object<O>& o,
    const std::string& argument)
{
    return colored_distance_glsl_function_name(o) + "("
    + argument + ")";
}
