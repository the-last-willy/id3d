#pragma once

#include <agl/engine/all.hpp>

struct FFD {
    agl::common::Grid<agl::Vec3> control_points;
};

inline
FFD ffd(
    std::array<std::size_t, 3> resolution,
    agl::common::Interval<3> domain)
{
    auto f = FFD();
    f.control_points = agl::common::Grid<agl::Vec3>(
        agl::common::grid_indexing(resolution));
    std::cout << lower_bound(domain) << " " << upper_bound(domain) << std::endl;
    for(std::size_t i = 0; i < resolution[0]; ++i)
    for(std::size_t j = 0; j < resolution[1]; ++j)
    for(std::size_t k = 0; k < resolution[2]; ++k) {
        auto r = agl::vec3(
            float(resolution[0]), float(resolution[1]), float(resolution[2]));
        auto t = agl::vec3(float(i), float(j), float(k)) / (r - agl::vec3(1.f));
        at(f.control_points, i, j, k) = mix(
            lower_bound(domain), upper_bound(domain), t);
    }
    return f;
}


