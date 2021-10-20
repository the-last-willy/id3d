#pragma once

#include <agl/engine/all.hpp>

#include <numeric>
#include <vector>

template<typename T>
struct Grid {
    std::vector<uint32_t> dimensions;
    std::vector<T> elements;

    Grid(std::vector<uint32_t> dimensions)
        : dimensions(std::move(dimensions))
    {
        auto sum = std::accumulate(begin(dimensions), end(dimensions), uint32_t(0));
        elements.resize(sum);
    }
};

template<typename G>
auto index(const Grid<G>& g, uint32_t i, uint32_t j) {
    return g.dimensions[0] * j + i;
}

template<typename G>
auto& at(Grid<G>& g, uint32_t i, uint32_t j) {
    return g.elements[index(g, i, j)];
}

struct Surface {
    uint32_t width;
    uint32_t height;

    std::vector<agl::Vec4> control_points;

    Surface(uint32_t w, uint32_t h)
        : width(w)
        , height(h)
    {
        control_points.resize(width * height);
    }
};

inline
agl::Vec4& control_point(Surface& s, uint32_t i, uint32_t j) {
    return s.control_points[j * s.width + i];
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

agl::Vec3 compute(const DeBoorsAlgorithm& dba, const Surface& s, float u, float v) {
    // Disable unused warnings.
    auto degree = s.control_points.size() - 1;

    (void) dba; (void) s; (void) u; (void) v;
    return agl::vec3(0.f);
}

// SURFACE OF REVOLUTION

// TWIST, TAPER -> NORMAL MATRIX

inline
agl::engine::TriangleMesh sampled_mesh(
    const Surface& s,
    uint32_t w,
    uint32_t h)
{
    auto g = Grid<agl::Vec3>({w, h});
    auto dba = DeBoorsAlgorithm(w, h);
    for(uint32_t i = 0; i < w; ++i)
    for(uint32_t j = 0; j < h; ++j) {
        auto u = float(i) / (w - 1.f);
        auto v = float(j) / (j - 1.f);
        at(g, i, j) = compute(dba, s, u, v);
    }
    
    auto m = agl::engine::TriangleMesh();


    return m;
}

inline
agl::engine::TriangleMesh control_mesh(const Surface& s) {
    auto cm = agl::engine::TriangleMesh();
    (void) s;
    // cm.topology
    return cm;
}
