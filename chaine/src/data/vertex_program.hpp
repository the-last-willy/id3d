#pragma once

#include <agl/engine/program.hpp>
#include <agl/engine/shader.hpp>

namespace chaine::data {

inline
auto vertex_program(const eng::ShaderCompiler& sc) {
    auto p = eng::Program();
    load(p, sc, {
        {
            agl::vertex_shader_tag,
            "shader/vertex.vs"
        }, {     
            agl::fragment_shader_tag,
            "shader/vertex.fs"
        }
    });
    p.capabilities.emplace_back(
        agl::Capability::depth_test,
        []() { glDepthFunc(GL_LESS); });
    p.capabilities.emplace_back(
        agl::Capability::program_point_size);
    return p;
}

}
