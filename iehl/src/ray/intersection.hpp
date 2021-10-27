#pragma once

#include <agl/engine/all.hpp>

struct Intersection {
    agl::engine::Hit hit;
    agl::engine::ConstFaceProxy triangle;
};
