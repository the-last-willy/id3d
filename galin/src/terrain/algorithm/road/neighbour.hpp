#pragma once

#include <limits>

const float max_weight = std::numeric_limits<float>::infinity();

struct neighbour {
    agl::Vec2 target;
    float weight;
};