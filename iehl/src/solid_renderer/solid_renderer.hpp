#pragma once

#include "opengl.hpp"

struct SolidRenderer {
    gl::Program program;

    // Attribute locations.

    gl::OptAttribLoc normal;
    gl::OptAttribLoc position;

    // Uniform locations.

    gl::OptUniformLoc rgba_color;
    gl::OptUniformLoc view_world_position;

    gl::OptUniformLoc object_to_clip_position;
    gl::OptUniformLoc object_to_world_normal;
    gl::OptUniformLoc object_to_world_position;
};

inline
SolidRenderer solid_renderer(eng::ShaderCompiler& sc) {
    auto sr = SolidRenderer();

    load(agl::Program(sr.program), sc, {
        {
            agl::vertex_shader_tag,
            "iehl/src/solid_renderer/draw.vert"
        }, {
            agl::fragment_shader_tag,
            "iehl/src/solid_renderer/draw.frag"
        }});

    // Attribute locations.

    sr.normal
    = gl::GetAttribLocation(sr.program,
        "a_normal");
    sr.position
    = gl::GetAttribLocation(sr.program,
        "a_position");

    // Uniform locations.

    sr.rgba_color
    = gl::GetUniformLocation(sr.program,
        "rgba_color");
    sr.view_world_position
    = gl::GetUniformLocation(sr.program,
        "view_world_position");
    
    sr.object_to_clip_position
    = gl::GetUniformLocation(sr.program,
        "object_to_clip_position");
    sr.object_to_world_normal
    = gl::GetUniformLocation(sr.program,
        "object_to_world_normal");
    sr.object_to_world_position
    = gl::GetUniformLocation(sr.program,
        "object_to_world_position");

    return sr;
}
