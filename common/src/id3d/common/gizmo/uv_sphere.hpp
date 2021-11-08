#pragma once

#include <agl/common/all.hpp>
#include <agl/engine/all.hpp>

#include <cmath>

namespace gizmo {

struct SphereInfo {

};

inline
agl::engine::TriangleMesh uv_sphere() {
    auto r = std::size_t(30);
    auto m = agl::engine::TriangleMesh();
    auto vertices = agl::common::Grid<agl::engine::MutableVertexProxy>(
        agl::common::grid_indexing({r, r}));
    for(std::size_t i = 0; i < r; ++i)
    for(std::size_t j = 0; j < r; ++j) {
        auto ai = float(i) / (r - 1) * agl::constant::tau;
        auto aj = float(j) / (r - 1) * agl::constant::pi;
        auto& v = at(vertices, i, j) = create_vertex(m);
        // position(v) = agl::vec3(
        //     std::sin(aj) * std::cos(ai),
        //     std::sin(aj) * std::sin(aj),
        //     std::cos(aj));
        position(v) = agl::vec3(
            std::sin(aj) * std::cos(ai),
            std::sin(aj) * std::sin(ai),
            std::cos(aj));
    }
    for(std::size_t i0 = 0; i0 < r; ++i0)
    for(std::size_t j0 = 0; j0 < r - 1; ++j0) {
        auto i1 = (i0 + 1) % r;
        auto j1 = j0 + 1;
        auto&& lf = create_face(m, 3); // Lower.
        topology(lf).incident_vertices[0] = index(at(vertices, i0, j0));
        topology(lf).incident_vertices[1] = index(at(vertices, i1, j0));
        topology(lf).incident_vertices[2] = index(at(vertices, i0, j1));
        auto&& uf = create_face(m, 3); // Upper.
        topology(uf).incident_vertices[0] = index(at(vertices, i1, j1));
        topology(uf).incident_vertices[1] = index(at(vertices, i0, j1));
        topology(uf).incident_vertices[2] = index(at(vertices, i1, j0));
    }
    return m;
}

}
