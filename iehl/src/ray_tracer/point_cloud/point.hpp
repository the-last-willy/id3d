#pragma once

#include <agl/glsl/all.hpp>

struct Point {
    using Color = agl::Vec3;
    using Normal = agl::Vec3;
    using Position = agl::Vec3;

    Color color;
    Normal normal;
    Position position;
};
