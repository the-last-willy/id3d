#pragma once

#include <agl/engine/all.hpp>

inline
agl::engine::TriangleMesh control_mesh1(const agl::common::Grid<agl::Vec3>& g) {
    auto m = agl::engine::TriangleMesh();
    auto vertices = agl::common::Grid<agl::engine::MutableVertexProxy>(indexing(g));
    for(uint32_t i = 0; i < size(g, 0); ++i) {
        auto&& v = at(vertices, i) = create_vertex(m);
        position(v) = at(g, i);
    }
    for(uint32_t i = 1; i < size(g, 0); ++i) {
        auto&& ft = topology(create_face(m, 2));
        ft.incident_vertices[0] = index(at(vertices, i - 1));
        ft.incident_vertices[1] = index(at(vertices, i));
    }
    return m;
}

inline
agl::engine::TriangleMesh control_mesh(const agl::common::Grid<agl::Vec3>& g) {
    auto m = agl::engine::TriangleMesh();
    auto vertices = agl::common::Grid<agl::engine::MutableVertexProxy>(indexing(g));
    for(uint32_t i = 0; i < size(g, 0); ++i)
    for(uint32_t j = 0; j < size(g, 1); ++j) {
        auto&& v = at(vertices, i, j) = create_vertex(m);
        position(v) = at(g, i, j);
    }
    for(uint32_t i = 1; i < size(g, 0); ++i)
    for(uint32_t j = 0; j < size(g, 1); ++j) {
        auto&& ft = topology(create_face(m, 2));
        ft.incident_vertices[0] = index(at(vertices, i - 1, j));
        ft.incident_vertices[1] = index(at(vertices, i, j));
    }
    for(uint32_t i = 0; i < size(g, 0); ++i)
    for(uint32_t j = 1; j < size(g, 1); ++j) {
        auto&& ft = topology(create_face(m, 2));
        ft.incident_vertices[0] = index(at(vertices, i, j - 1));
        ft.incident_vertices[1] = index(at(vertices, i, j));
    }
    return m;
}

inline
agl::engine::TriangleMesh control_mesh3(const agl::common::Grid<agl::Vec3>& g) {
    auto m = agl::engine::TriangleMesh();
    auto vertices = agl::common::Grid<agl::engine::MutableVertexProxy>(indexing(g));
    for(uint32_t i = 0; i < size(g, 0); ++i)
    for(uint32_t j = 0; j < size(g, 1); ++j)
    for(uint32_t k = 0; k < size(g, 2); ++k) {
        auto&& v = at(vertices, i, j, k) = create_vertex(m);
        position(v) = at(g, i, j, k);
    }
    for(uint32_t i = 1; i < size(g, 0); ++i)
    for(uint32_t j = 0; j < size(g, 1); ++j)
    for(uint32_t k = 0; k < size(g, 2); ++k) {
        auto&& ft = topology(create_face(m, 2));
        ft.incident_vertices[0] = index(at(vertices, i - 1, j, k));
        ft.incident_vertices[1] = index(at(vertices, i, j, k));
    }
    for(uint32_t i = 0; i < size(g, 0); ++i)
    for(uint32_t j = 1; j < size(g, 1); ++j)
    for(uint32_t k = 0; k < size(g, 2); ++k) {
        auto&& ft = topology(create_face(m, 2));
        ft.incident_vertices[0] = index(at(vertices, i, j - 1, k));
        ft.incident_vertices[1] = index(at(vertices, i, j, k));
    }
    for(uint32_t i = 0; i < size(g, 0); ++i)
    for(uint32_t j = 0; j < size(g, 1); ++j)
    for(uint32_t k = 1; k < size(g, 2); ++k) {
        auto&& ft = topology(create_face(m, 2));
        ft.incident_vertices[0] = index(at(vertices, i, j, k - 1));
        ft.incident_vertices[1] = index(at(vertices, i, j, k));
    }
    return m;
}
