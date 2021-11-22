#pragma once

#include "terrain.hpp"

inline
auto& domain(const Terrain& t) {
    return t.settings.domain;
}
