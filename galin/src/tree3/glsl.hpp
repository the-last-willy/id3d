#pragma once

#include <array>
#include <sstream>

inline
bool contains(const std::string& s, char c) {
    for(auto sc : s) {
        if(sc == c) {
            return true;
        }
    }
    return false;
}

template<typename T>
std::string glsl(const T& t) {
    auto ss = std::stringstream();
    ss << t;
    auto r = ss.str();
    if(contains(r, '.')) {
        return r;
    } else {
        return r + ".";
    }
}

template<std::size_t N>
std::string glsl(const std::array<float, N>& a) {
    auto ss = std::stringstream();
    ss << "vec"
    << N << "("
    << a[0] << ", "
    << a[1] << ", "
    << a[2] << ")";
    return ss.str();
}
