#pragma once

#include "constant.hpp"

#include <array>
#include <sstream>

template<std::size_t N>
std::string glsl_expression(const Constant<std::array<float, N>>& c) {
    auto ss = std::stringstream();
    ss << "vec"
    << N << "("
    << c.value[0] << ", "
    << c.value[1] << ", "
    << c.value[2] << ")";
    return ss.str();
}

template<typename C>
std::string glsl_expression(const Constant<C>& c) {
    auto ss = std::stringstream();
    ss << c.value;
    return ss.str();
}

template<typename C, typename... Ts>
std::string glsl_expression(const Constant<C>& c, Ts&&...) {
    return glsl_expression(c);
}
