#pragma once

#include <agl/engine/all.hpp>

#include <array>

struct TerrainSettings {
    agl::common::Interval<agl::Vec2> domain;
    std::array<std::size_t, 2> resolution;
};
