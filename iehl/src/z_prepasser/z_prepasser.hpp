#pragma once

#include "opengl.hpp"

#include <agl/engine/all.hpp>

struct Z_Prepasser {
    gl::Program program;

    // Attribute locations.

    gl::OptAttribLoc world_position;

    // Uniform locations.

    gl::OptUniformLoc world_to_clip;
};

inline
Z_Prepasser z_prepasser(eng::ShaderCompiler& sc) {
    auto zp = Z_Prepasser();

    load(agl::Program(zp.program), sc, {
        {
            agl::vertex_shader_tag,
            "iehl/src/z_prepasser/draw.vert"
        }, {
            agl::fragment_shader_tag,
            "iehl/src/z_prepasser/draw.frag"
        }});
    
    // Attribute locations.

    zp.world_position = gl::GetAttribLocation(zp.program,
        "a_world_position");

    // Uniform locations.

    zp.world_to_clip = gl::GetUniformLocation(zp.program,
        "world_to_clip");

    return zp;
}
