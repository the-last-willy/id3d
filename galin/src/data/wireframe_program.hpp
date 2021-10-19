#pragma once

#include <agl/engine/all.hpp>

namespace data {

inline
auto wireframe_program(eng::ShaderCompiler& sc) {
    auto p = eng::Program();
    load(p, sc, {
        {
            agl::vertex_shader_tag,
            "/mesh/wireframe.vert"
        }, {
            agl::fragment_shader_tag,
            "/mesh/wireframe.frag"
        }
    });
    p.capabilities.emplace_back(
        agl::Capability::depth_test,
        []() { glDepthFunc(GL_LEQUAL); });
    return p;
}

}
