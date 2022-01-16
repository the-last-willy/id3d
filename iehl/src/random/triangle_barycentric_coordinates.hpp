#pragma once

#include <array>
#include <cmath>

inline
std::array<float, 3> random_triangle_barycentric_coordinates(
    float r1,
    float r2)
{
    auto sqrt_r1 = std::sqrt(r1);
    auto u = 1.f - sqrt_r1;
    auto v = (1.f - r2) * sqrt_r1;
    auto w = r2 * sqrt_r1;
    return {u, v, w};
}
