#pragma once

#include "field/all.hpp"
#include "drainage_functions.hpp"
#include "terrain.hpp"
#include "update_cpu.hpp"
#include "update_gpu.hpp"

inline
void update_all(Terrain& t) {
    steepest_compute(t);
    update_laplacian(t);
    update_gradient(t);
    update_normals(t);
    update_slope(t);
    update_colors(t);
    update_cpu(t);
    update_gpu(t);
}

