#pragma once

#include "proxy.hpp"
#include "vertex.hpp"

#include "face_vertex/proxy/vertex/position.hpp"

#include <agl/opengl/all.hpp>

namespace face_vertex {

inline
bool contains_xy(TriangleProxy tp, agl::Vec3 v) {
    auto p0 = position(vertex(tp, 0));
    auto p1 = position(vertex(tp, 1));
    auto p2 = position(vertex(tp, 2));
    // Signed aread.
    auto a0 = cross(p1 - p0, v - p0)[2];
    auto a1 = cross(p2 - p1, v - p1)[2];
    auto a2 = cross(p0 - p2, v - p2)[2];
    return a0 > 0.f and a1 > 0.f and a2 > 0.f;
}

}
