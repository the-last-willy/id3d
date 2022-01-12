#pragma once

#include "opengl.hpp"

#include <vector>

struct FrustumCuller {
    gl::Program program;

    // Shader storage buffer bindings.

    GLuint input_draw_commands_buffer_binding = 0;
    GLuint object_world_bounds_buffer_binding = 1;
    GLuint output_draw_command_count_buffer_binding = 2;
    GLuint output_draw_commands_buffer_binding = 3;

    // Uniform locations.

    GLint frustum_clip_bounds_0_uniform_location = -1;
    GLint frustum_clip_bounds_1_uniform_location = -1;
    GLint frustum_world_bounds_0_uniform_location = -1;
    GLint frustum_world_bounds_1_uniform_location = -1;
    GLint world_to_clip_uniform_location = -1;
};

inline
FrustumCuller frustum_culler(eng::ShaderCompiler& sc) {
    auto fc = FrustumCuller();

    // Programs.

    load(agl::Program(fc.program), sc, {
        {
            agl::compute_shader_tag,
            "iehl/src/frustum_culler/cull.comp"
        }});

    // Shader storage buffer bindigs.

    gl::ShaderStorageBlockBinding(fc.program,
        "input_draw_commands_buffer",
        fc.input_draw_commands_buffer_binding);
    gl::ShaderStorageBlockBinding(fc.program,
        "output_draw_command_count_buffer",
        fc.output_draw_command_count_buffer_binding);
    gl::ShaderStorageBlockBinding(fc.program,
        "output_draw_commands_buffer",
        fc.output_draw_commands_buffer_binding);
    gl::ShaderStorageBlockBinding(fc.program,
        "object_world_bounds_buffer",
        fc.object_world_bounds_buffer_binding);

    // Uniform locations.

    fc.frustum_clip_bounds_0_uniform_location
    = glGetUniformLocation(fc.program,
        "frustum_clip_bounds.b[0]");
    fc.frustum_clip_bounds_1_uniform_location
    = glGetUniformLocation(fc.program,
        "frustum_clip_bounds.b[1]");
    fc.frustum_world_bounds_0_uniform_location
    = glGetUniformLocation(fc.program,
        "frustum_world_bounds.b[0]");
    fc.frustum_world_bounds_1_uniform_location
    = glGetUniformLocation(fc.program,
        "frustum_world_bounds.b[1]");
    fc.world_to_clip_uniform_location
    = glGetUniformLocation(fc.program,
        "world_to_clip");

    return fc;
}
