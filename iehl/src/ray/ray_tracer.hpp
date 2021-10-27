#pragma once

#include <agl/engine/all.hpp>

#include <random>

struct RayTracer {
    agl::Vec3 position;
    agl::Mat4 rotation;

    std::default_random_engine* random;

    std::uniform_real_distribution<float> x_distribution;
    std::uniform_real_distribution<float> y_distribution;
};
