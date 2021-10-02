#pragma once

#include "tree2/node/node.hpp"

#include <sstream>

template<typename L, typename R>
struct Union : Node<L, R> {
    Union(L l, R r)
        : Node<L, R>(std::move(l), std::move(r))
    {}
};

template<typename L, typename R>
Union(L, R) -> Union<L, R>;

template<typename... U>
std::string glsl_expression(const Union<U...>& u, const std::string& arg) {
    auto ss = std::stringstream();
    ss << "min("
    << glsl_expression(std::get<0>(u.children), arg) << ", "
    << glsl_expression(std::get<1>(u.children), arg) << ")";
    return ss.str();
}
