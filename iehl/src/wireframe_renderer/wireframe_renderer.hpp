#pragma once

#include "opengl.hpp"

#include <agl/engine/all.hpp>

struct WireframeRenderer {
    gl::Program program;

    // Uniform locations.

    GLint model_to_clip_uniform_location = -1;
    GLint rgba_color_uniform_location = -1;

    // Vertex attribute locations.

    GLint position_attribute_location = -1;
};

inline
WireframeRenderer wireframe_renderer(eng::ShaderCompiler& sc) {
    auto wr = WireframeRenderer();

    load(agl::Program(wr.program), sc, {
        {
            agl::vertex_shader_tag,
            "iehl/src/wireframe_renderer/shader.vert"
        }, {
            agl::fragment_shader_tag,
            "iehl/src/wireframe_renderer/shader.frag"
        }});

    // Uniform locations.

    wr.model_to_clip_uniform_location
    = glGetUniformLocation(wr.program, "model_to_clip");
    wr.rgba_color_uniform_location
    = glGetUniformLocation(wr.program, "rgba_color");

    // Vertex attribute locations.

    wr.position_attribute_location
    = glGetAttribLocation(wr.program, "a_position");

    return wr;
}
