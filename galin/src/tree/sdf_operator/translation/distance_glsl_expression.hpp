#pragma once

#include "translation.hpp"

#include "tree/glsl_expression.hpp"

#include <string>

inline
std::string distance_glsl_expression(
    const Translation& t,
    const std::string& argument)
{
    return "(" + argument + " - " + glsl_expression(t.value) + ")";
}
