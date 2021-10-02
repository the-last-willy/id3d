#pragma once

#include "tree2/node/node.hpp"

#include <array>
#include <sstream>

struct Plane : Node<> {};

inline
std::string glsl_expression(const Plane&, const std::string& arg) {
    auto ss = std::stringstream();
    ss << "sdf_plane(" 
    << arg <<  ")";
    return ss.str();
}
