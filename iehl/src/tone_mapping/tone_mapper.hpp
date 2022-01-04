#pragma once

#include "opengl.hpp"

#include <agl/engine/all.hpp>

struct ToneMapper {
    gl::Program program;

    // Texture units.

    GLuint hdr_color_texture_unit = 0;

    // Uniform locations.

    GLint hdr_color_texture_uniform_location = -1;
};

inline
ToneMapper tone_mapper(eng::ShaderCompiler& sc) {
    auto tm = ToneMapper();

    load(agl::Program(tm.program.name), sc, {
        {
            agl::vertex_shader_tag,
            "iehl/src/tone_mapping/shader.vert"
        }, {
            agl::fragment_shader_tag,
            "iehl/src/tone_mapping/shader.frag"
        }});

    // Uniform locations.

    tm.hdr_color_texture_uniform_location
    = glGetUniformLocation(tm.program, "hdr_color_texture");

    return tm;
}
