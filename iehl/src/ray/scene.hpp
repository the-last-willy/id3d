#pragma once

#include <agl/engine/all.hpp>

struct Scene {
    std::vector<agl::engine::ConstFaceProxy> all_faces;
    std::vector<agl::engine::ConstFaceProxy> emissive_faces;
};
