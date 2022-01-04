#pragma once

#include "hdr_framebuffer/all.hpp"
#include "tone_mapper.hpp"

inline
void bind(const ToneMapper& tm) {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glUseProgram(tm.program);
}

inline
void bind(const ToneMapper& tm, const HDR_Framebuffer& fb) {
    // Textures.

    glBindTextureUnit(
        tm.hdr_color_texture_unit,
        fb.color_texture);

    // Uniforms.

    glProgramUniform1i(tm.program,
        tm.hdr_color_texture_uniform_location,
        tm.hdr_color_texture_unit);
}
