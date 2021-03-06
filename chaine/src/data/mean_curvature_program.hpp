#pragma once

#include <agl/engine/program.hpp>
#include <agl/engine/shader.hpp>

namespace data {

inline
auto mean_curvature_program(const eng::ShaderCompiler& sc) {
    auto p = eng::Program();
    load(p, sc, {
        {
            agl::vertex_shader_tag,
            "shader/triangle.vs"
        }, {     
            agl::fragment_shader_tag,
            "shader/mean_curvature.fs"
        }
    });
    p.capabilities.emplace_back(
        agl::Capability::cull_face, 
        []() { glCullFace(GL_BACK); });
    p.capabilities.emplace_back(
        agl::Capability::depth_test, 
        []() { glDepthFunc(GL_LESS); });
    return p;
}

}
