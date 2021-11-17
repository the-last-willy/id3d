#pragma once

#include "terrain.hpp"

inline
auto create(TerrainSettings ts) {
    auto t = Terrain();
    {
        t.settings = ts;
    }
    auto nx = resolution(t)[0];
    auto ny = resolution(t)[1];
    { // Data.
        t.heights = agl::common::grid<float>(nx, ny);
        t.colors = agl::common::grid<agl::Vec3>(nx, ny);
        for(auto& h : t.heights) {
            h = 0.f;
        }
        for(auto& c : t.colors) {
            c = agl::vec3(1.f, 0.f, 0.f);
        }

        compute_data(t);
    }
    { // CPU mesh.
        auto mapping = index_to_world_mapping(t);
        t.cpu_mesh = std::make_unique<agl::engine::TriangleMesh>();
        t.vertices = agl::common::grid<agl::engine::MutableVertexProxy>(nx, ny);
        for(std::size_t i = 0; i < nx; ++i)
        for(std::size_t j = 0; j < ny; ++j) {
            auto&& v = at(t.vertices, i, j) = create_vertex(*t.cpu_mesh);
            auto xy = mapping(agl::vec2(float(i), float(j)));
            position(v) = agl::vec3(xy[0], 0.f, xy[1]);

        }
        for(std::size_t i = 1; i < nx; ++i)
        for(std::size_t j = 1; j < ny; ++j) {
            // Lower triangle.
            auto&& lf = create_face(*t.cpu_mesh, 3);
            topology(lf).incident_vertices[0] 
            = index(at(t.vertices, i - 1, j - 1));
            topology(lf).incident_vertices[1] 
            = index(at(t.vertices, i - 1, j    ));
            topology(lf).incident_vertices[2] 
            = index(at(t.vertices, i    , j - 1));
            // Upper triangle.
            auto&& uf = create_face(*t.cpu_mesh, 3);
            topology(uf).incident_vertices[0] 
            = index(at(t.vertices, i    , j    ));
            topology(uf).incident_vertices[1] 
            = index(at(t.vertices, i    , j - 1));
            topology(uf).incident_vertices[2] 
            = index(at(t.vertices, i - 1, j    ));
        }
    }
    { // GPU.
        update_gpu(t);
    }
    return t;
}

inline
void create(Terrain& t, std::size_t nx, std::size_t ny) {
    auto ts = TerrainSettings();
    ts.domain = agl::common::interval(
        agl::vec2(0.f),
        agl::vec2(float(nx - 1), float(ny - 1)));
    ts.resolution = {nx, ny};
    t = create(ts);
}
