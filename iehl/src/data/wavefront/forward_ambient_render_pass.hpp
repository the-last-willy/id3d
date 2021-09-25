#pragma once

#include "forward_ambient_program.hpp"

#include <agl/engine/all.hpp>

namespace iehl::data::wavefront {

inline
auto forward_ambient_render_pass(eng::ShaderCompiler& sc) {
    auto rp = agl::engine::RenderPass();
    rp.program = std::make_shared<eng::Program>(forward_ambient_program(sc));
    return rp;
}

}
