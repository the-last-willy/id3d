#pragma once

#include "terrain/all.hpp"

#include <agl/common/all.hpp>

#include <filesystem>
#include <fstream>

inline
auto load_srtm1(const std::filesystem::path& file_path) {
    std::cout << "Loading srtm." << std::endl;

    float scaling = 1'000.f;

    auto ts = TerrainSettings();
    ts.domain = agl::common::interval(agl::vec2(0.f), agl::vec2(3600.f * 30.f / scaling));
    ts.resolution = {3601, 3601};
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



    for(std::size_t i = 0; i < resolution(t)[0]; ++i)
    for(std::size_t j = 0; j < resolution(t)[1]; ++j) {
        at(t.heights, i, j) = at(heights, i, j);
    }

    return t;
}
