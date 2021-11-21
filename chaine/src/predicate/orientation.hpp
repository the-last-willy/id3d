#pragma once

#include <agl/opengl/all.hpp>

inline
bool orientation(agl::Vec2 v0, agl::Vec2 v1) {
    return cross(v0, v1) >= 0.f;
}

inline
bool orientation(agl::Vec2 origin, agl::Vec2 p0, agl::Vec2 p1) {
    return orientation(p0 - origin, p1 - origin);
}

inline
bool orientation(agl::Vec3 v0, agl::Vec3 v1, agl::Vec3 v2) {
    return dot(cross(v0, v1), v2) >= 0.f;
}

inline
float orientation(agl::Vec3 origin, agl::Vec3 p0, agl::Vec3 p1, agl::Vec3 p2) {
    return orientation(p0 - origin, p1 - origin, p2 - origin);
}
