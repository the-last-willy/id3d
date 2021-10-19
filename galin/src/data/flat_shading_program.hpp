#pragma once

#include <agl/engine/all.hpp>

namespace data {

inline
auto flat_shading_program(eng::ShaderCompiler& sc) {
    auto p = eng::Program();
    load(p, sc, {
        {
            agl::vertex_shader_tag,
            "/mesh/flat_shading.vert"
        }, {
            agl::fragment_shader_tag,
            "/mesh/flat_shading.frag"
        }
    });
    p.capabilities.emplace_back(agl::Capability::cull_face, []() {
        glCullFace(GL_BACK); });
    p.capabilities.emplace_back(agl::Capability::depth_test, []() {
        glDepthFunc(GL_LEQUAL); });
    return p;
}

}
