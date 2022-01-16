#pragma once

#include "ray_tracer.hpp"

inline
void bind(const RayTracer& rt) {
    glUseProgram(rt.draw_program);
}
