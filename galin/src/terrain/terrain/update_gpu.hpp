#pragma once

#include "terrain.hpp"

inline
void update_gpu(Terrain& t) {
    t.gpu_mesh = agl::standard::shared(
        agl::engine::instance(
            agl::engine::triangle_mesh(
                *t.cpu_mesh, {})));
}
