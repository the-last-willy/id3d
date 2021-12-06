#pragma once

#include "field/field.hpp"
#include "settings.hpp"

#include <array>

struct Terrain {
    TerrainSettings settings;

    // Data.

    Field<agl::Vec3> color;
    Field<float> drainage_area;
    Field<agl::Vec2> gradient;
    Field<float> height;
    Field<float> laplacian;
    Field<agl::Vec3> normal;
    Field<float> slope;
    Field<std::array<float, 3>> vegetation_probability;
    Field<float> wetnessIndex;

    // CPU.

    agl::common::Grid<agl::engine::MutableVertexProxy> vertices;

    std::unique_ptr<agl::engine::TriangleMesh> cpu_mesh;

    // GPU.

    std::shared_ptr<agl::engine::MeshInstance> gpu_mesh;  
};
