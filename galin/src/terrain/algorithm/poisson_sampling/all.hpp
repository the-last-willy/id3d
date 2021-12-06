#pragma once

#include <agl/common/all.hpp>
#include <agl/standard/all.hpp>

#include <random>
#include <vector>

template<typename T> constexpr
auto clamp(T x, T min, T max) {
    if(x <= min) {
        return min;
    } else if(x >= max) {
        return max;
    } else {
        return x;
    }
}

inline
auto poisson_sampling2(
    agl::common::Interval<agl::Vec2> domain,
    float r)
{
    auto l = length(domain);

    auto rand = std::default_random_engine(
        agl::standard::random_seed());
    auto d0 = std::uniform_real_distribution<float>(
        lower_bound(domain)[0], upper_bound(domain)[0]);
    auto d1 = std::uniform_real_distribution<float>(
        lower_bound(domain)[1], upper_bound(domain)[1]);

    auto n0 = std::size_t(std::ceil(l[0] / r));
    auto n1 = std::size_t(std::ceil(l[1] / r));

    auto samples = std::vector<agl::Vec2>();
    auto grid = agl::common::grid(n0, n1);

    for(std::size_t it = 0; it < 100; ++it) {
        auto s = agl::vec2(d0(rand), d1(rand));


        for(int i0 = std::max(); i0 < )
    }
}
