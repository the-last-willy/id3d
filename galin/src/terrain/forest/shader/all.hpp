#pragma once

#include <agl/engine/all.hpp>
#include <agl/standard/all.hpp>

inline
auto trees_shader(eng::ShaderCompiler& sc) {
    auto p = agl::opengl::Program();
    load(agl::Program(p.name), sc, {
        {
            agl::vertex_shader_tag,
            "galin/src/terrain/forest/shader/shader.vert"
        },
        {
            agl::fragment_shader_tag,
            "galin/src/terrain/forest/shader/shader.frag"
        }});
    return p;
}
