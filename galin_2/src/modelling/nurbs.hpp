#pragma once

#include "control_mesh.hpp"

#include <agl/engine/all.hpp>

#include <functional>
#include <numeric>
#include <vector>

template<typename G>
auto bezier(const agl::common::Grid<G>& g, float u) {
    if(dimension(g) != 1) {
        throw std::logic_error("Bezier curve: wrong dimension.");
    }
    auto tmp = std::vector<agl::Vec3>(size(g, 0));
    for(uint32_t i = 0; i < size(g, 0); ++i) {
        tmp[i] = at(g, i);
    }
    for(uint32_t i = 0; i < size(g, 0) - 1; ++i) {
        for(uint32_t j = 1; j < size(g, 0) - i; ++j) {
            tmp[j - 1] = (1.f - u) * tmp[j - 1] + u * tmp[j];
        }
    }
    return tmp[0];
}

template<typename G>
auto bezier(const agl::common::Grid<G>& g, float u, float v) {
    if(dimension(g) != 2) {
        throw std::logic_error("Bezier surface: wrong dimension.");
    }
    auto sub = agl::common::Grid<G>(agl::common::grid_indexing(size(g, 0)));
    auto tmp = std::vector<agl::Vec3>(size(g, 1));
    for(uint32_t i = 0; i < size(g, 0); ++i) {
        for(uint32_t j = 0; j < size(g, 1); ++j) {
            tmp[j] = at(g, i, j);
        }
        for(uint32_t j = 0; j < size(g, 1) - 1; ++j) {
            for(uint32_t k = 1; k < size(g, 1) - j; ++k) {
                tmp[k - 1] = (1.f - v) * tmp[k - 1] + v * tmp[k];
            }
        }
        at(sub, i) = tmp[0];
        // std::cout << "v[" << i << "]" << tmp[0] << "\n";
    }
    return bezier(sub, u);
}

template<typename G>
auto bezier(const agl::common::Grid<G>& g, float u, float v, float w) {
    if(dimension(g) != 3) {
        throw std::logic_error("Bezier volume: wrong dimension.");
    }
    auto sub = agl::common::Grid<G>(agl::common::grid_indexing(size(g, 0), size(g, 1)));
    auto tmp = std::vector<agl::Vec3>(size(g, 2));
    for(uint32_t h = 0; h < size(g, 0); ++h)
    for(uint32_t i = 0; i < size(g, 1); ++i) {
        for(uint32_t j = 0; j < size(g, 2); ++j) {
            tmp[j] = at(g, h, i, j);
        }
        for(uint32_t j = 0; j < size(g, 2) - 1; ++j) {
            for(uint32_t k = 1; k < size(g, 2) - j; ++k) {
                tmp[k - 1] = (1.f - w) * tmp[k - 1] + w * tmp[k];
            }
        }
        at(sub, h, i) = tmp[0];
        // std::cout << "w[" << h << "," << i << "]" << tmp[0] << "\n";
    }
    return bezier(sub, u, v);
}

template<typename T> constexpr
T binomial_coefficient(T k, T n) {
    auto bc = T(1);
    for(T i = n - k + 1; i <= n; ++i) {
        bc *= i;
    }
    for(T i = 1; i <= k; ++i) {
        bc /= i;
    }
    return bc;
}

struct DeBoorsAlgorithm {
    uint32_t width;
    uint32_t height;
};

// SURFACE OF REVOLUTION

inline
agl::engine::TriangleMesh sampled_mesh(
    const agl::common::Grid<agl::Vec3>& g,
    uint32_t w,
    uint32_t h)
{
    auto m = agl::engine::TriangleMesh();
    auto vertices = agl::common::Grid<agl::engine::MutableVertexProxy>(
        agl::common::grid_indexing({w, h}));
    for(uint32_t i = 0; i < w; ++i)
    for(uint32_t j = 0; j < h; ++j) {
        auto&& v = at(vertices, i, j) = create_vertex(m);
        position(v) = bezier(g, float(i) / (w - 1), float(j) / (h - 1));
    }
    for(uint32_t i = 1; i < w; ++i)
    for(uint32_t j = 1; j < h; ++j) {
        auto&& ft0 = topology(create_face(m, 3));
        ft0.incident_vertices[0] = index(at(vertices, i - 1, j - 1));
        ft0.incident_vertices[1] = index(at(vertices, i    , j - 1));
        ft0.incident_vertices[2] = index(at(vertices, i - 1, j    ));
        auto&& ft1 = topology(create_face(m, 3));
        ft1.incident_vertices[0] = index(at(vertices, i    , j    ));
        ft1.incident_vertices[1] = index(at(vertices, i - 1, j    ));
        ft1.incident_vertices[2] = index(at(vertices, i    , j - 1));
    }
    return m;
}

// inline
// agl::engine::TriangleMesh sampled_mesh(
//     const agl::common::Grid<agl::Vec4>& g,
//     uint32_t w,
//     uint32_t h)
// {
//     auto m = agl::engine::TriangleMesh();
//     auto vertices = agl::common::Grid<agl::engine::MutableVertexProxy>(
//         agl::common::grid_indexing({w, h}));
//     for(uint32_t i = 0; i < w; ++i)
//     for(uint32_t j = 0; j < h; ++j) {
//         auto&& v = at(vertices, i, j) = create_vertex(m);
//         position(v) = bezier(g, float(i) / (w - 1), float(j) / (h - 1));
//     }
//     for(uint32_t i = 1; i < w; ++i)
//     for(uint32_t j = 1; j < h; ++j) {
//         auto&& ft0 = topology(create_face(m, 3));
//         ft0.incident_vertices[0] = index(at(vertices, i - 1, j - 1));
//         ft0.incident_vertices[1] = index(at(vertices, i    , j - 1));
//         ft0.incident_vertices[2] = index(at(vertices, i - 1, j    ));
//         auto&& ft1 = topology(create_face(m, 3));
//         ft1.incident_vertices[0] = index(at(vertices, i    , j    ));
//         ft1.incident_vertices[1] = index(at(vertices, i - 1, j    ));
//         ft1.incident_vertices[2] = index(at(vertices, i    , j - 1));
//     }
//     return m;
// }

template<typename Curve>
agl::engine::TriangleMesh revolution_surface(
    const Curve& c,
    const agl::Mat4& tr,
    uint32_t s0,
    uint32_t s1)
{
    auto itr = inverse(tr);
    auto m = agl::engine::TriangleMesh();
    auto vertices = agGrid<agl::engine::MutableVertexProxy>(agl::common::grid_indexing(s0, s1));
    for(uint32_t i = 0; i < s1; ++i) {
        auto angle = 2.f * agl::constant::pi * i / s1;
        auto rev = tr * agl::rotation_y(angle) * itr;
        for(uint32_t j = 0; j < s0; ++j) {
            auto&& v = at(vertices, i, j) = create_vertex(m);
            position(v) = (rev * vec4(bezier(c, float(j) / (s0 - 1)), 1.f)).xyz();
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
    for(uint32_t j = 1; j < s1; ++j) {
        auto&& ft0 = topology(create_face(m, 3));
        ft0.incident_vertices[0] = index(at(vertices, s0 - 1, j - 1));
        ft0.incident_vertices[1] = index(at(vertices, 0     , j - 1));
        ft0.incident_vertices[2] = index(at(vertices, s0 - 1, j    ));
        auto&& ft1 = topology(create_face(m, 3));
        ft1.incident_vertices[0] = index(at(vertices, 0     , j    ));
        ft1.incident_vertices[1] = index(at(vertices, s0 - 1, j    ));
        ft1.incident_vertices[2] = index(at(vertices, 0     , j - 1));
    }
    return m;
}

