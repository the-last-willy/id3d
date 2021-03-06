#pragma once

#include "bezier/all.hpp"

#include <agl/engine/all.hpp>

inline
agl::engine::TriangleMesh revolution_surface(
    const agl::common::Grid<agl::Vec3>& c,
    const agl::Mat4& tr,
    uint32_t s0,
    uint32_t s1)
{
    auto itr = inverse(tr);
    auto m = agl::engine::TriangleMesh();
    auto vertices = agl::common::Grid<agl::engine::MutableVertexProxy>(
        agl::common::grid_indexing(s0, s1));
    for(uint32_t j = 0; j < s1; ++j) {
        auto angle = 2.f * agl::constant::pi * j / s1;
        auto rev = tr * agl::rotation_y(angle) * itr;
        for(uint32_t i = 0; i < s0; ++i) {
            auto&& v = at(vertices, i, j) = create_vertex(m);
            auto ph = bezier_rational(c, float(i) / (s0 - 1));
            position(v) = (rev * agl::vec4(ph[0], ph[1], 0.f, 1.f)).xyz();
        }
    }
    
    for(uint32_t i = 1; i < s0; ++i)
    for(uint32_t j = 1; j < s1; ++j) {
        auto&& ft0 = topology(create_face(m, 3));
        ft0.incident_vertices[0] = index(at(vertices, i - 1, j - 1));
        ft0.incident_vertices[1] = index(at(vertices, i    , j - 1));
        ft0.incident_vertices[2] = index(at(vertices, i - 1, j    ));
        auto&& ft1 = topology(create_face(m, 3));
        ft1.incident_vertices[0] = index(at(vertices, i    , j    ));
        ft1.incident_vertices[1] = index(at(vertices, i - 1, j    ));
        ft1.incident_vertices[2] = index(at(vertices, i    , j - 1));
    }
    for(uint32_t i = 1; i < s0; ++i) {
        auto&& ft0 = topology(create_face(m, 3));
        ft0.incident_vertices[0] = index(at(vertices, i - 1, s1 - 1));
        ft0.incident_vertices[2] = index(at(vertices, i    , s1 - 1));
        ft0.incident_vertices[1] = index(at(vertices, i - 1, 0     ));
        auto&& ft1 = topology(create_face(m, 3));
        ft1.incident_vertices[0] = index(at(vertices, i    , 0     ));
        ft1.incident_vertices[2] = index(at(vertices, i - 1, 0     ));
        ft1.incident_vertices[1] = index(at(vertices, i    , s1 - 1));
    }
    return m;
}
