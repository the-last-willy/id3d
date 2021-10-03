#pragma once

#include "combination.hpp"

#include <string>
#include <tuple>

template<typename... C>
std::string distance_glsl_expression(
    const Combination<C...>& c,
    const std::string& argument)
{
    // Hardcoded arity.
    if constexpr(std::tuple_size_v<decltype(c.operands)> == 0) {
        static_assert(false, "Not implemented.");
    } else if constexpr(std::tuple_size_v<decltype(c.operands)> == 1) {
        return glsl_expression(
            c.operator_.distance,
            distance_glsl_expression(std::get<0>(c.operands), argument));
    } else if constexpr(std::tuple_size_v<decltype(c.operands)> == 2) {
        return glsl_expression(
            c.operator_.distance,
            distance_glsl_expression(std::get<0>(c.operands), argument),
            distance_glsl_expression(std::get<1>(c.operands), argument));
    }
}
