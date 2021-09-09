#pragma once

#include <agl/engine/program.hpp>
#include <agl/engine/shader.hpp>

namespace chaine::data {

inline
auto triangle_program(const eng::ShaderCompiler& sc) {
    auto p = eng::Program();
    load(p, sc, {
        {
            agl::vertex_shader_tag,
            "chaine/shader/triangle.vs"
        }, {     
            agl::fragment_shader_tag,
            "chaine/shader/triangle.fs"
        }
    });
    p.capabilities.emplace_back(
        agl::Capability::cull_face, 
        []() { glCullFace(GL_FRONT); });
    p.capabilities.emplace_back(
        agl::Capability::depth_test, 
        []() { glDepthFunc(GL_LESS); });
    return p;
}

}
