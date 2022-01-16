#pragma once

#include "point_cloud/all.hpp"
#include "bind.hpp"
#include "ray_tracer.hpp"
#include "ray.hpp"
#include "trace.hpp"

inline
RayTracer ray_tracer(eng::ShaderCompiler& sc) {
    auto rt = RayTracer();

    // Programs.

    load(agl::Program(rt.draw_program), sc, {
        {
            agl::vertex_shader_tag,
            "iehl/src/ray_tracer/draw.vert"
        }, {
            agl::fragment_shader_tag,
            "iehl/src/ray_tracer/draw.frag"
        }});

    // Attribute locations.

    rt.draw_color = gl::GetAttribLocation(rt.draw_program,
        "a_color");
    rt.draw_normal = gl::GetAttribLocation(rt.draw_program,
        "a_normal");
    rt.draw_position = gl::GetAttribLocation(rt.draw_program,
        "a_position");
    
    // Uniform locations.

    rt.draw_object_to_clip_position = gl::GetUniformLocation(rt.draw_program,
        "object_to_clip_position");
    rt.draw_object_to_world_normal = gl::GetUniformLocation(rt.draw_program,
        "object_to_world_normal");
    rt.draw_object_to_world_position = gl::GetUniformLocation(rt.draw_program,
        "object_to_world_position");
    rt.draw_view_world_position = gl::GetUniformLocation(rt.draw_program,
        "view_world_position");

    // Vertex arrays.

    rt.pc.vao = vertex_array(rt.pc, rt);

    return rt;
}
