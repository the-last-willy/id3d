#pragma once

#include <agl/common/all.hpp>
#include <agl/engine/all.hpp>

#include <cmath>

namespace gizmo {

struct SphereInfo {

};

inline
agl::engine::TriangleMesh uv_sphere(SphereInfo si) {
    auto r = std::size_t(30);

    auto m = agl::engine::TriangleMesh();
    auto vertices = agl::common::Grid<agl::engine::MutableVertexProxy>(
        agl::common::GridIndexing({r, r}));
    for(std::size_t i = 0; i < r; ++i)
    for(std::size_t j = 0; j < r; ++j) {
        auto ai = float(i) / (r - 1) * agl::constant::tau;
        auto aj = float(i) / (r - 1) * agl::constant::pi;
        auto& v = at(vertices, i, j) = create_vertex(m);
        position(v) = agl::vec3(
            std::sin(aj) * std::cos(ai),
            std::sin(aj) * std::sin(aj),
            std::cos(aj));
    }
    for(std::size_t i = 0; i < r; ++i)
    for(std::size_t j = 0; j < r - 1; ++j) {
        auto&& lf = create_face(m, 3); // Lower.
        topology(f).incident_vertex[0] = at(vertices, i, j);
        topology(f).incident_vertex[1] = at(vertices, (i + 1) % r, j);
        topology(f).incident_vertex[2] = at(vertices, i, j + 1);
        auto&& uf = create_face(m, 3); // Upper.
        topology(f).incident_vertex[0] = at(vertices, i, j);
        topology(f).incident_vertex[1] = at(vertices, i, j);
        topology(f).incident_vertex[2] = at(vertices, i, j);
    }
    return m;
}

}
