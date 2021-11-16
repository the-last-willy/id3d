#pragma once

#include <agl/engine/all.hpp>

inline
eng::Program frustrum_culling_tester(eng::ShaderCompiler& sc) {
    auto p = eng::Program();
    load(p, sc, {
        {
            agl::compute_shader_tag,
            "iehl/src/shader/frustrum_culling/test.comp"
        }});
    return p;
}
