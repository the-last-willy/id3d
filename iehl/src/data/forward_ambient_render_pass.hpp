#pragma once

#include <agl/engine/all.hpp>

namespace iehl::data {

inline
auto forward_ambient_render_pass(eng::ShaderCompiler& sc) {
    auto rp = eng::RenderPass();
    rp.program = std::make_shared<eng::Program>();
    eng::load(*rp.program, sc, {
        {
            agl::vertex_shader_tag,
            "forward/ambient.vs"
        }, {
            agl::fragment_shader_tag,
            "forward/ambient.fs"
        }
    });
    return rp;
}

}
