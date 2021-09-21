#pragma once

#include "proxy.hpp"
#include "vertex.hpp"

#include <agl/all.hpp>

namespace face_vertex {

inline
auto barycenter(TriangleProxy tp) {
    auto p0 = position(vertex(tp, 0));
    auto p1 = position(vertex(tp, 1));
    auto p2 = position(vertex(tp, 2));
    return (p0 + p1 + p2) / 3.f;
}

}
