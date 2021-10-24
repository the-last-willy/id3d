#pragma once

#include <agl/engine/all.hpp>

#include <functional>
#include <numeric>
#include <vector>

template<typename T>
struct Grid {
    std::vector<uint32_t> dimensions;
    std::vector<T> elements;

    Grid(std::vector<uint32_t> dimensions)
        : dimensions(std::move(dimensions))
    {
        auto count = std::reduce(
            begin(this->dimensions),
            end(this->dimensions),
            uint32_t(1),
            std::multiplies<>());
        elements.resize(count);
    }
};

template<typename G>
auto index(const Grid<G>& g, uint32_t i, uint32_t j) {
    return g.dimensions[0] * j + i;
}

template<typename G>
const auto& at(const Grid<G>& g, uint32_t i, uint32_t j) {
    return g.elements[index(g, i, j)];
}

template<typename G>
auto& at(Grid<G>& g, uint32_t i, uint32_t j) {
    return g.elements[index(g, i, j)];
}

template<typename G>
auto& size(const Grid<G>& g) {
    return g.dimensions;
}

template<typename G>
uint32_t size(const Grid<G>& g, uint32_t d) {
    return g.dimensions[d];
}

template<typename G>
auto bezier(const Grid<G>& g, float u, float v) {
    auto tmp0 = std::vector<agl::Vec3>(size(g, 0));
    auto tmp1 = std::vector<agl::Vec3>(size(g, 1));
    for(uint32_t i = 0; i < size(g, 0); ++i) {
        for(uint32_t j = 0; j < size(g, 1); ++j) {
            tmp1[j] = at(g, i, j);
        }
        for(uint32_t j = 0; j < size(g, 1) - 1; ++j) {
            for(uint32_t k = 1; k < size(g, 1) - j; ++k) {
                tmp1[k - 1] = (1.f - v) * tmp1[k - 1] + v * tmp1[k];
            }
        }
        tmp0[i] = tmp1[0];
    }
    for(uint32_t j = 0; j < size(g, 1) - 1; ++j) {
        for(uint32_t k = 1; k < size(g, 1) - j; ++k) {
            tmp0[k - 1] = (1.f - u) * tmp0[k - 1] + u * tmp0[k];
        }
    }
    return tmp0[0];
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

// TWIST, TAPER -> NORMAL MATRIX

inline
agl::engine::TriangleMesh sampled_mesh(
    const Grid<agl::Vec3>& g,
    uint32_t w,
    uint32_t h)
{
    auto m = agl::engine::TriangleMesh();
    auto vertices = Grid<agl::engine::MutableVertexProxy>({w, h});
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

inline
agl::engine::TriangleMesh control_mesh(const Grid<agl::Vec3>& g) {
    auto m = agl::engine::TriangleMesh();
    auto vertices = Grid<agl::engine::MutableVertexProxy>(size(g));
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
