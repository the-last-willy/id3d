#pragma once

#include "distance_to.hpp"

#include <sstream>

template<typename DT>
std::string glsl_expression(
    const DistanceTo<DT>& dt,
    const std::string& argument)
{
    auto ss = std::stringstream();
    ss << glsl_distance_function_name(std::get<0>(dt.children)) << "("
    << argument << ")";
    return ss.str();
}
