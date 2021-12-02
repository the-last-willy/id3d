#pragma once

#include <agl/engine/all.hpp>
#include "terrain.hpp"

struct Drainage_data {
    std::array<size_t, 2> position;
    float height;
};

bool compare_height(const Drainage_data &a, const Drainage_data &b) {
    return a.height > b.height;
}
