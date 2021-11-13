#pragma once

#include "tree2/node/node.hpp"

#include <array>
#include <sstream>

struct Cube : Node<> {};

inline
std::string glsl_expression(const Cube&, const std::string& arg) {
    auto ss = std::stringstream();
    ss << "sdf_cube(" 
    << arg <<  ")";
    return ss.str();
}
