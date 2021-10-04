#pragma once

#include <agl/opengl/all.hpp>

inline
float orientation(agl::Vec2 v0, agl::Vec2 v1, agl::Vec2 v2) {
    auto d1 = agl::vec3(v1 - v0, 0.f);
    auto d2 = agl::vec3(v2 - v0, 0.f);
    return cross(d1, d2)[2];
}
