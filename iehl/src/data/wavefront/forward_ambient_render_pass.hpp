#pragma once

#include <agl/engine/all.hpp>

namespace iehl::data::wavefront {

inline
auto forward_ambient_render_pass(eng::ShaderCompiler& sc) {
    auto rp = agl::engine::RenderPass();
    auto& program = *(rp.program = std::make_shared<eng::Program>());
    load(program, sc, {
        {
            agl::vertex_shader_tag,
            "/wavefront/forward_ambient.vs"
        }, {
            agl::fragment_shader_tag,
            "/wavefront/forward_ambient.fs"
        }
    });
    program.capabilities.emplace_back(agl::Capability::cull_face, []() {
        glCullFace(GL_FRONT); });
    program.capabilities.emplace_back(agl::Capability::depth_test, []() {
        glDepthFunc(GL_LEQUAL); });
    return rp;
}

}
