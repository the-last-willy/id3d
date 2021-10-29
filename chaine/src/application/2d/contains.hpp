#pragma once

#include "predicate/all.hpp"

#include <agl/opengl/all.hpp>

namespace face_vertex {

inline
bool contains(TriangleProxy tp, agl::Vec2 v) {
    auto p0 = position(vertex(tp, 0)).xy();
    auto p1 = position(vertex(tp, 1)).xy();
    auto p2 = position(vertex(tp, 2)).xy();
    return orientation(v, p0, p1)
    and orientation(v, p1, p2)
    and orientation(v, p2, p0);
}

}
