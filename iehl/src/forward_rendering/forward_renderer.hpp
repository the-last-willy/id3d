#pragma once

#include "shader/render/all.hpp"
#include "opengl.hpp"

struct ForwardRenderer {
    gl::Program program;

    GLuint normal_attribute_location = 0;
    GLuint position_attribute_location = 1;
    GLuint texcoords_attribute_location = 2;
};

inline
auto forward_renderer(eng::ShaderCompiler& sc) {
    auto fr = ForwardRenderer();
    load(fr.program, sc, {
        {
            agl::vertex_shader_tag,
            "/iehl/src/forward_rendering/shader.vert"
        }, {
            agl::fragment_shader_tag,
            "/iehl/src/forward_rendering/shader.frag"
        }});
    return fr;
}
