#pragma once

#include "sphere.hpp"
#include "tree/glsl_expression.hpp"

inline
std::string distance_glsl_expression(const SDF_Sphere& s, const std::string& argument) {
    auto ss = std::stringstream();
    ss << "(length(" 
    << argument <<  ") - " 
    << glsl_expression(s.radius) << ")";
    return ss.str();
}
