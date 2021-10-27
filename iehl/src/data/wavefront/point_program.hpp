#pragma once

#include <agl/engine/all.hpp>

namespace data::wavefront {

inline
eng::Program point_program(eng::ShaderCompiler& sc) {
    auto p = eng::Program();
    load(p, sc, {
        {
            agl::vertex_shader_tag,
            "/wavefront/point.vert"
        }, {
            agl::fragment_shader_tag,
            "/wavefront/point.frag"
        }
    });
    p.capabilities.emplace_back(agl::Capability::depth_test, []() {
        glDepthFunc(GL_LEQUAL); });
    return p;
}

}
