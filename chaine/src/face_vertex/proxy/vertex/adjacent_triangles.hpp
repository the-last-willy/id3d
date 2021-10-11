#pragma once

#include "index.hpp"
#include "proxy.hpp"
#include "topology.hpp"

#include "face_vertex/proxy/triangle/proxy.hpp"

namespace face_vertex {

template<typename I, typename S>
struct IteratorAndSentinel {
    I iterator;
    S sentinel;
};

template<typename I, typename S>
IteratorAndSentinel(I, S) -> IteratorAndSentinel<I, S>;

template<typename I, typename S> constexpr
I begin(const IteratorAndSentinel<I, S>& ias) {
    return ias.iterator;
}

template<typename I, typename S> constexpr
S end(const IteratorAndSentinel<I, S>& ias) {
    return ias.sentinel;
}

struct VertexAdjacentTriangleIterator {
    VertexIndex common_vertex;
    TriangleIndex first_triangle;
    TriangleProxy current_triangle;
    bool done = false;
};

struct EmptySentinel {};

inline
auto operator*(const VertexAdjacentTriangleIterator& i) {
    return i.current_triangle;
}

inline
auto& operator++(VertexAdjacentTriangleIterator& i) {
    auto& t = i.current_triangle;
    auto& vs = topology(t).vertices;
    for(uint32_t j = 0; j < vertex_count(t); ++j) {
        if(vs[j] == i.common_vertex) {
            i.current_triangle = adjacent_triangle(t, (j + 1) % vertex_count(t));
            if(index(i.current_triangle) == i.first_triangle) {
                i.done = true;
            }
            return i;
        }
    }
    throw std::logic_error(
        "Not a triangle index.");
}

constexpr
bool operator!=(const VertexAdjacentTriangleIterator& i, EmptySentinel) {
    return not i.done;
}

inline
auto adjacent_triangles(VertexProxy vp) {
    auto it = VertexAdjacentTriangleIterator{
        .common_vertex = index(vp),
        .first_triangle = topology(vp).triangle,
        .current_triangle = proxy(mesh(vp), topology(vp).triangle)};
    return IteratorAndSentinel{std::move(it), EmptySentinel()};
}

}
