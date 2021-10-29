#pragma once

#include <agl/opengl/all.hpp>

inline
agl::Vec3 paraboloid(float x, float y) {
    return agl::vec3(x, y, x * x + y * y);
}

inline
agl::Vec3 paraboloid(agl::Vec2 v) {
    return paraboloid(v[0], v[1]);
}
