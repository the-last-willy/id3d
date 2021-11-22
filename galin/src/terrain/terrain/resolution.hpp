#pragma once

#include "terrain.hpp"

inline
auto& resolution(const Terrain& t) {
    return t.settings.resolution;
}
