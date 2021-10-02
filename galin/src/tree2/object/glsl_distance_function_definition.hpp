#pragma once

#include "glsl_distance_function_name.hpp"
#include "object.hpp"

template<typename... O>
std::string glsl_distance_function_definition(const Object<O...>& o) {
    auto ss = std::stringstream();
    ss << "float " << glsl_distance_function_name(o) << "(vec3 position) {\n";
    ss << "\treturn " << glsl_expression(std::get<0>(o.children), "position") << ";\n";
    ss << "}\n";
    return ss.str();
}
