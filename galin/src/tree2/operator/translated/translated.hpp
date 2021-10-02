#pragma once

#include "tree2/node/node.hpp"

#include <array>
#include <sstream>

template<typename Child>
struct Translated : Node<Child> {
    std::array<float, 3> translation = {0.f, 0.f, 0.f};

    constexpr
    Translated(Child c, std::array<float, 3> translation)
        : Node<Child>(std::move(c))
        , translation(translation)
    {}
};

template<typename Child>
Translated(Child, std::array<float, 3>) -> Translated<Child>;

template<typename T>
std::string glsl_expression(const Translated<T>& t, const std::string& argument) {
    auto ass = std::stringstream();
    ass << "(" << argument << " - vec3(" 
    << t.translation[0] << ", "
    << t.translation[1] << ", "
    << t.translation[2] << "))";

    auto ss = std::stringstream();
    ss << glsl_expression(std::get<0>(t.children), ass.str());
    return ss.str();
}
