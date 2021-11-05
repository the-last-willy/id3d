#pragma once

#include "dependencies.hpp"

namespace data::shader {

inline
eng::Program wireframe(eng::ShaderCompiler& sc) {
    auto p = eng::Program();
    load(p, sc, {
        {
            agl::vertex_shader_tag,
            "/wireframe.vert"
        }, {
            agl::fragment_shader_tag,
            "/wireframe.frag"
        }
    });
    p.capabilities.emplace_back(agl::Capability::depth_test, []() {
        glDepthFunc(GL_LEQUAL); });
    return p;
}

}
