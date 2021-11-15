#pragma once

#include <agl/engine/all.hpp>

#include <array>
#include <memory>

struct Terrain {
    //

    agl::common::Interval<2> domain;
    std::array<std::size_t, 2> resolution;

    // Data.

    agl::common::Grid<float> heights;

    // CPU.

    agl::common::Grid<agl::engine::MutableVertexProxy> vertices;

    std::unique_ptr<agl::engine::TriangleMesh> cpu_mesh;

    // GPU.

    std::shared_ptr<agl::engine::MeshInstance> gpu_mesh;  
};

inline
void update_gpu(Terrain& t) {
    t.gpu_mesh = agl::standard::shared(
        agl::engine::instance(
            agl::engine::triangle_mesh(
                *t.cpu_mesh, {})));
}

inline
void create(Terrain& t, std::size_t nx, std::size_t ny) {
    {
        t.domain = agl::common::interval(
            agl::vec2(0.f), agl::vec2(float(nx), float(ny)));
        t.resolution = {nx, ny};
    }
    { // Data.
        t.heights = agl::common::grid<float>(nx, ny);
        for(auto& h : t.heights) {
            h = 0.f;
        }
    }
    { // CPU mesh.
        t.cpu_mesh = std::make_unique<agl::engine::TriangleMesh>();
        t.vertices = agl::common::grid<agl::engine::MutableVertexProxy>(nx, ny);
        for(std::size_t i = 0; i < nx; ++i)
        for(std::size_t j = 0; j < ny; ++j) {
            auto&& v = at(t.vertices, i, j) = create_vertex(*t.cpu_mesh);
            position(v) = agl::vec3(float(i) / 10.f, 0.f, float(j) / 10.f);
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
}

inline
void update_cpu(Terrain& t) {
    for(std::size_t i = 0; i < t.resolution[0]; ++i)
    for(std::size_t j = 0; j < t.resolution[1]; ++j) {
        position(at(t.vertices, i, j))[1] = at(t.heights, i, j);
    }

}


