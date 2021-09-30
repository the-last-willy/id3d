#pragma once

#include <sstream>

template<typename L, typename R>
struct Union {
    L lhs;
    R rhs;
};

template<typename L, typename R>
Union(L, R) -> Union<L, R>;

template<typename... U>
std::string glsl_expr(const Union<U...>& u, const std::string& arg) {
    auto ss = std::stringstream();
    ss << "min(" << glsl_expr(u.lhs, arg) << ", " << glsl_expr(u.rhs, arg) << ")";
    return ss.str();
}
