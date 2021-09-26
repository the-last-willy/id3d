#pragma once

#include <agl/engine/all.hpp>

namespace data::wavefront {

inline
eng::Program forward_blinn_phong_program(eng::ShaderCompiler& sc) {
    auto p = eng::Program();
    load(p, sc, {
        {
            agl::vertex_shader_tag,
            "/wavefront/blinn_phong.vs"
        }, {
            agl::fragment_shader_tag,
            "/wavefront/blinn_phong.fs"
        }
    });
    p.capabilities.emplace_back(agl::Capability::blend, []() {
        glBlendFunc(GL_ONE, GL_ONE); });
    p.capabilities.emplace_back(agl::Capability::cull_face, []() {
        glCullFace(GL_BACK); });
    p.capabilities.emplace_back(agl::Capability::depth_test, []() {
        glDepthFunc(GL_LEQUAL); });
    return p;
}

}
