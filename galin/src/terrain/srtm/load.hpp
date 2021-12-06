#pragma once

#include "terrain/all.hpp"

#include <agl/common/all.hpp>

#include <filesystem>
#include <fstream>

inline
auto load_srtm1(
    const std::filesystem::path& file_path,
    std::array<std::size_t, 2> position = {0, 0},
    std::array<std::size_t, 2> size = {3601, 3601})
{
    std::cout << "Loading srtm." << std::endl;

    float scaling = 1'000.f;

    auto ts = TerrainSettings();
    ts.domain = agl::common::interval(
        agl::vec2(float(position[0]), float(position[1])),
        agl::vec2(float(position[0] + size[0]), float(position[1] + size[1])));
    lower_bound(ts.domain) *= 30.f / scaling;
    upper_bound(ts.domain) *= 30.f / scaling;
    ts.resolution = {size[0], size[1]};
    auto t = create(ts);

    auto f = std::ifstream(
        file_path.c_str(),
        std::ios::binary | std::ios::in);

    if(not f.is_open()) {
        throw std::runtime_error(
            "Failed to open \"" + file_path.string() + "\".");
    }

    auto heights = agl::common::grid<float>(3601, 3601);

    std::cout << "Reading values." << std::endl;

    for(std::size_t i = 0; i < 3601; ++i)
    for(std::size_t j = 0; j < 3601; ++j) {
        auto most = f.get();
        auto least = f.get();
        auto h = std::uint16_t(256 * most + least);
        at(heights, i, j) = float(h) / scaling;
    }

    auto h = value_accessor(t.height);

    for(std::size_t i = 0; i < size[0]; ++i)
    for(std::size_t j = 0; j < size[1]; ++j) {
        h(i, j) = at(heights, position[0] + i, position[1] + j);
    }

    return t;
}
