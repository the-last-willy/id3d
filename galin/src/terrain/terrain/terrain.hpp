#pragma once

#include "settings.hpp"

struct Terrain {
    //

    TerrainSettings settings;

    // Data.

    agl::common::Grid<agl::Vec2> gradients;
    agl::common::Grid<float> laplacian;
    agl::common::Grid<float> heights;
    agl::common::Grid<agl::Vec3> normals;
    agl::common::Grid<float> slopes;
    agl::common::Grid<float> drainage_areas;
    agl::common::Grid<agl::Vec3> colors;

    // CPU.

    agl::common::Grid<agl::engine::MutableVertexProxy> vertices;

    std::unique_ptr<agl::engine::TriangleMesh> cpu_mesh;

    // GPU.

    std::shared_ptr<agl::engine::MeshInstance> gpu_mesh;  
};
