#pragma once

#include "tree2/node/node.hpp"

#include <sstream>

template<typename Child>
struct Scaled : Node<Child> {
    float scaling = 1.f;

    constexpr
    Scaled(Child c, float scaling)
        : Node<Child>(std::move(c))
        , scaling(scaling)
    {}
};

template<typename Child>
Scaled(Child, float) -> Scaled<Child>;

template<typename S>
std::string glsl_expression(const Scaled<S>& s, const std::string& argument) {
    auto arg = std::stringstream();
    arg << "(" << argument << " / " << s.scaling << ")";

    auto ss = std::stringstream();
    ss << "(" 
    << glsl_expression(std::get<0>(s.children), arg.str()) << " * "
    << s.scaling << ")";
    return ss.str();
}
