#pragma once

#include "object/all.hpp"
#include "occlusion_culler.hpp"

inline
void bind_for_drawing(const OcclusionCuller& oc) {
    glBindFramebuffer(GL_FRAMEBUFFER, oc.depth_fbo);
    glUseProgram(oc.draw_program);
}

inline
void bind_for_testing(const OcclusionCuller& oc) {
    glUseProgram(oc.test_program);
}
