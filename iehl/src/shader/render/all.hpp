#pragma once

#include <agl/engine/all.hpp>

inline
auto render_program(eng::ShaderCompiler& sc) {
    auto p = eng::Program();
    load(p, sc, {
        {
            agl::vertex_shader_tag,
            "/iehl/src/shader/render/shader.vert"
        }, {
            agl::fragment_shader_tag,
            "/iehl/src/shader/render/shader.frag"
        }
    });
    return p;
}
