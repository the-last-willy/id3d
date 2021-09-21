#pragma once

#include <agl/engine/program.hpp>
#include <agl/engine/shader.hpp>

namespace data {

inline
auto edge_program(const eng::ShaderCompiler& sc) {
    auto p = eng::Program();
    load(p, sc, {
        {
            agl::vertex_shader_tag,
            "shader/edge.vs"
        }, {     
            agl::fragment_shader_tag,
            "shader/edge.fs"
        }
    });
    p.capabilities.emplace_back(
        agl::Capability::depth_test,
        []() { glDepthFunc(GL_LEQUAL); });
    return p;
}

}
