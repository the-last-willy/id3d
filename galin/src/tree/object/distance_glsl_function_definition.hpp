#pragma once

#include "distance_glsl_expression.hpp"
#include "distance_glsl_function_name.hpp"
#include "object.hpp"

#include <string>

template<typename O>
std::string distance_glsl_function_definition(const Object<O>& o) {
    return 
    "float " + distance_glsl_function_name(o) + "(vec3 position) {\n"
    + "    return " + distance_glsl_expression(o.content, "position") + ";\n"
    + "}\n";
}
