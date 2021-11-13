#pragma once

#include "tree2/glsl_expression.hpp"
#include "constant.hpp"

#include <array>
#include <sstream>

template<typename C, typename... Ts>
std::string glsl_expression(const Constant<C>& c, Ts&&...) {
    return glsl_expression(c);
}
