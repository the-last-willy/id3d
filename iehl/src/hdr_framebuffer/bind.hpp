#pragma once

#include "hdr_framebuffer.hpp"

inline
void bind(const HDR_Framebuffer& fb) {
    glBindFramebuffer(GL_FRAMEBUFFER, fb.fbo);
}
