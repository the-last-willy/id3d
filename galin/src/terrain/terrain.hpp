#pragma once

#include <agl/engine/all.hpp>

#include <array>
#include <memory>

struct TerrainSettings {
    agl::common::Interval<2> domain;
    std::array<std::size_t, 2> resolution;
};

struct Terrain {
    //

    TerrainSettings settings;

    // Data.

    agl::common::Grid<float> heights;
    agl::common::Grid<agl::Vec2> gradients;
    agl::common::Grid<agl::Vec3> normals;
    agl::common::Grid<float> slopes;

    // CPU.

    agl::common::Grid<agl::engine::MutableVertexProxy> vertices;

    std::unique_ptr<agl::engine::TriangleMesh> cpu_mesh;

    // GPU.

    std::shared_ptr<agl::engine::MeshInstance> gpu_mesh;  
};

inline
auto& resolution(const Terrain& t) {
    return t.settings.resolution;
}

inline
auto& domain(const Terrain& t) {
    return t.settings.domain;
}

inline
agl::Vec2 delta(const Terrain& t) {
    auto n = agl::vec2(float(resolution(t)[0] - 1), float(resolution(t)[1] - 1));
    return length(domain(t)) / n;
}

inline
auto index_to_world_mapping(const Terrain& t) {
    auto n = agl::vec2(
        float(resolution(t)[0] - 1),
        float(resolution(t)[1] - 1));
    auto a = (length(domain(t))) / n;
    auto b = lower_bound(domain(t));
    return [a, b](agl::Vec2 v) {
        return a * v + b;
    };
}

inline
void update_gpu(Terrain& t) {
    t.gpu_mesh = agl::standard::shared(
        agl::engine::instance(
            agl::engine::triangle_mesh(
                *t.cpu_mesh, {})));
}

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
        for(auto& h : t.heights) {
            h = 0.f;
        }

        t.gradients = agl::common::grid<agl::Vec2>(nx, ny);
        auto dx = 1;
        auto dy = 1;
        for(std::size_t i = 1; i < nx - 2; ++i) {
            for(std::size_t j = 1; j < ny; ++j) {
                auto dfdx = (at(t.heights, i + 1, j) - at(t.heights, i - 1, j)) / (2 * dx);
                auto dfdy = (at(t.heights, i, j + 1) - at(t.heights, i, j - 1)) / (2 * dy);
                at(t.gradients, i, j) = agl::vec2(dfdx, dfdy);
            }
        }
        for(size_t j = 1; j < ny - 2; ++j) {
            //replace 0 by domain's lower bound
            auto left_dfdx = (at(t.heights, 1, j) - at(t.heights, 0, j)) / dx;
            auto right_dfdx = (at(t.heights, nx - 1, j) - at(t.heights, nx - 2, j)) / dx;
            auto left_dfdy = (at(t.heights, 0, j + 1) - at(t.heights, 0, j - 1)) / (2 * dy);
            auto right_dfdy = (at(t.heights, nx - 1, j + 1) - at(t.heights, nx - 1, j - 1)) / (2 * dy);
            at(t.gradients, 0, j) = agl::vec2(left_dfdx, left_dfdy);
            at(t.gradients, nx - 1, j) = agl::vec2(right_dfdx, right_dfdy);
        }
        for(size_t i = 1; i < nx - 2; ++i) {
            //replace 0 by domain's lower bound
            auto up_dfdx = (at(t.heights, i + 1, ny - 1) - at(t.heights, i - 1, ny - 1)) / (2 * dx);
            auto down_dfdx = (at(t.heights, i + 1, 0) - at(t.heights, i - 1, 0)) / (2 * dx);
            auto up_dfdy = (at(t.heights, 1, ny - 1) - at(t.heights, 0, ny - 2)) / dy;
            auto down_dfdy = (at(t.heights, i, 1) - at(t.heights, i, 0)) / dy;
            at(t.gradients, i, ny - 1) = agl::vec2(up_dfdx, up_dfdy);
            at(t.gradients, i, 0) = agl::vec2(down_dfdx, down_dfdy);
        }
        // corner
        at(t.gradients, 0, 0) = agl::vec2(
                                            (at(t.heights, 1, 0) - at(t.heights, 0, 0)) / dx,
                                            (at(t.heights, 0, 1) - at(t.heights, 0, 0)) / dy
                                        );
        at(t.gradients, nx - 1, 0) = agl::vec2(
                                            (at(t.heights, nx - 1, 0) - at(t.heights, nx - 2, 0)) / dx,
                                            (at(t.heights, nx - 1, 1) - at(t.heights, nx - 1, 0)) / dy
                                        );
        at(t.gradients, 0, ny - 1) = agl::vec2(
                                            (at(t.heights, 1, ny - 1) - at(t.heights, 0, ny - 1)) / dx,
                                            (at(t.heights, 0, ny - 1) - at(t.heights, 0, ny - 2)) / dy
                                        );
        at(t.gradients, nx - 1, ny - 1) = agl::vec2(
                                            (at(t.heights, nx - 1, ny - 1) - at(t.heights, nx - 2, ny - 1)) / dx,
                                            (at(t.heights, nx - 1, ny - 1) - at(t.heights, nx - 1, ny - 2)) / dy
                                        );

        t.normals = agl::common::grid<agl::Vec3>(nx, ny);
        t.slopes = agl::common::grid<float>(nx, ny);
        for(std::size_t i = 0; i < nx; ++i) {
            for(std::size_t j = 0; j < ny; ++j) {
                at(t.normals, i, j) = agl::vec3(-at(t.gradients, i, j), 1);
                at(t.slopes, i, j) = agl::length(at(t.gradients, i, j));
            }
        }
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

inline
void update_cpu(Terrain& t) {
    for(std::size_t i = 0; i < resolution(t)[0]; ++i)
    for(std::size_t j = 0; j < resolution(t)[1]; ++j) {
        position(at(t.vertices, i, j))[1] = at(t.heights, i, j);
    }

}

