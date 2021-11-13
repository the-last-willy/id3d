#pragma once

#include <array>
#include <sstream>

template<typename T>
std::string glsl_expression(const T& t) {
    auto ss = std::stringstream();
    ss << t;
    return ss.str();
}

template<std::size_t N>
std::string glsl_expression(const std::array<float, N>& a) {
    auto ss = std::stringstream();
    ss << "vec"
    << N << "("
    << a[0] << ", "
    << a[1] << ", "
    << a[2] << ")";
    return ss.str();
}
