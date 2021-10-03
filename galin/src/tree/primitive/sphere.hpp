#pragma once

#include "tree2/node/node.hpp"

#include <array>
#include <sstream>

struct Sphere : Node<> {
    float radius = 1.f;

    Sphere(float radius)
        : radius(radius)
    {}
};

inline
std::string glsl_expression(const Sphere& p, const std::string& arg) {
    auto ss = std::stringstream();
    ss << "(length(" 
    << arg <<  ") - " 
    << p.radius << ")";
    return ss.str();
}
