#pragma once

#include "delta.hpp"
#include "resolution.hpp"
#include "terrain.hpp"
#include "update_gpu.hpp"

inline
auto index_to_domain_mapping(const Terrain& t) {
    auto n = agl::vec2(
        float(resolution(t)[0] - 1),
        float(resolution(t)[1] - 1));
    auto factor = length(domain(t)) / n;
    auto offset = lower_bound(domain(t));
    return [factor, offset](agl::Vec2 v) {
        return factor * v + offset;
    };
}

inline
auto create(TerrainSettings ts) {
    auto t = Terrain();
    t.settings = ts;

    auto nx = resolution(t)[0];
    auto ny = resolution(t)[1];
    { // Fields.
        t.color = field(nx, ny, agl::vec3(0.f));
        t.drainage_area = field(nx, ny, 0.f);
        t.gradient = field(nx, ny, agl::vec2(0.f));
        t.height = field(nx, ny, 0.f);
        t.laplacian = field(nx, ny, 0.f);
        t.normal = field(nx, ny, agl::vec3(0.f));
        t.slope = field(nx, ny, 0.f);
        t.vegetation_probability = field(nx, ny, std::array{0.f, 0.f, 0.f});
        t.wetness = field(nx, ny, 0.f);

        auto da = value_accessor(t.drainage_area);
        for(std::size_t i = 0; i < nx; ++i) {
            for(std::size_t j = 0; j < ny; ++j) {
                da(i, j) = 1.f;
            }
        }
    }
    { // CPU mesh.
        auto mapping = index_to_domain_mapping(t);
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
