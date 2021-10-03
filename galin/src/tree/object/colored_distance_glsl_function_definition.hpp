#pragma once

#include "colored_distance_glsl_expression.hpp"
#include "colored_distance_glsl_function_name.hpp"
#include "object.hpp"

#include <string>

template<typename O>
std::string colored_distance_glsl_function_definition(const Object<O>& o) {
    return 
    "ColoredDistance " + colored_distance_glsl_function_name(o) + "(vec3 position) {\n"
    + "    return " + colored_distance_glsl_expression(o.content, "position") + ";\n"
    + "}\n";
}
