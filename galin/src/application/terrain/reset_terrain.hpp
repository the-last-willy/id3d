#pragma once

#include "application.hpp"
#include "update_terrain.hpp"

inline
void reset_terrain(App& a) {
    a.terrain = load_srtm1(
        "./galin/data/srtm1/N45E005.hgt",
        {1000, 500},
        {500, 500});
    a.settings.is_height_outdated = true;
    update_terrain(a);
}
