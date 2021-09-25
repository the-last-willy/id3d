#pragma once

#include <agl/engine/all.hpp>

namespace iehl::data::wavefront {

inline
auto forward_ambient_program(eng::ShaderCompiler& sc) {
    auto p = eng::Program();
    load(p, sc, {
        {
            agl::vertex_shader_tag,
            "/wavefront/forward_ambient.vs"
        }, {
            agl::fragment_shader_tag,
            "/wavefront/forward_ambient.fs"
        }
    });
    p.capabilities.emplace_back(agl::Capability::cull_face, []() {
        glCullFace(GL_BACK); });
    p.capabilities.emplace_back(agl::Capability::depth_test, []() {
        glDepthFunc(GL_LEQUAL); });
    return p;
}

}
