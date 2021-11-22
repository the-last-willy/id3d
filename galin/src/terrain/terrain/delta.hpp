#pragma once

#include "domain.hpp"
#include "resolution.hpp"
#include "terrain.hpp"

inline
agl::Vec2 delta(const Terrain& t) {
    auto n = agl::vec2(float(resolution(t)[0] - 1), float(resolution(t)[1] - 1));
    return length(domain(t)) / n;
}
