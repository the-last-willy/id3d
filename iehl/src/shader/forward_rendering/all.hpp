#pragma once

#include <agl/engine/all.hpp>

inline
auto forward_rendering_program(eng::ShaderCompiler& sc) {
    auto p = eng::Program();
    load(p, sc, {
        {
            agl::vertex_shader_tag,
            "/iehl/src/shader/forward_rendering/shader.vert"
        }, {
            agl::fragment_shader_tag,
            "/iehl/src/shader/forward_rendering/shader.frag"
        }
    });
    return p;
}
