#pragma once

#include "proxy.hpp"
#include "vertex.hpp"

#include "face_vertex/proxy/vertex/position.hpp"

#include <agl/opengl/all.hpp>

namespace face_vertex {

inline
auto area(TriangleProxy tp) {
    auto v0 = position(vertex(tp, 0));
    auto v1 = position(vertex(tp, 1));
    auto v2 = position(vertex(tp, 2));
    return length(cross(v0 - v1, v0 - v2)) / 2.f;
}

}
