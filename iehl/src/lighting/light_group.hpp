#pragma once

#include "light.hpp"

#include <agl/engine/opengl/all.hpp>

#include <vector>

struct LightGroup {
    std::vector<Light> lights;
    agl::engine::opengl::Buffer light_ssbo;
};

inline
void init_ssbos(LightGroup& lg) {
    glNamedBufferStorage(
        lg.light_ssbo,
        size(lg.lights) * sizeof(Light),
        data(lg.lights),
        GL_NONE);
}
