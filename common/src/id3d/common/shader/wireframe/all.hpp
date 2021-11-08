#pragma once

#include <agl/engine/all.hpp>

namespace shader {

inline
eng::Program wireframe(eng::ShaderCompiler& sc) {
    auto p = eng::Program();
    load(p, sc, {
        {
            agl::vertex_shader_tag,
            "common/src/id3d/common/shader/wireframe/shader.vert"
        }, {
            agl::fragment_shader_tag,
            "common/src/id3d/common/shader/wireframe/shader.frag"
        }
    });
    p.capabilities.emplace_back(
        agl::Capability::depth_test,
        []() { glDepthFunc(GL_LEQUAL); });
    return p;
}

}
