#pragma once

#include <agl/engine/all.hpp>

inline
eng::Program frustum_culling_shader(eng::ShaderCompiler& sc) {
    auto p = eng::Program();
    load(p, sc, {
        {
            agl::compute_shader_tag,
            "iehl/src/shader/frustum_culling/shader.comp"
        }});
    return p;
}
