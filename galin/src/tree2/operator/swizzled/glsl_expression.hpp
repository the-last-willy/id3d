#pragma once

#include "swizzled.hpp"

#include <array>
#include <sstream>

template<typename T>
std::string glsl_expression(const Swizzled<T>& s, const std::string& argument) {
    auto symbols = std::array{'x', 'y', 'z', 'w'};
    auto ss = std::stringstream();
    ss << argument << ".";
    for(auto&& sw : s.swizzling) {
        ss << symbols.at(sw);
    }
    return glsl_expression(std::get<0>(s.children), ss.str());
}
