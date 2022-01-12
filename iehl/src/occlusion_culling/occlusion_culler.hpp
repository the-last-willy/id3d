#pragma once

#include "opengl.hpp"

#include <vector>

struct OcclusionCuller {
    GLsizei width = 512;
    GLsizei height = 256;

    gl::Framebuffer depth_fbo;

    std::vector<GLint> depth_image_widths;
    std::vector<GLint> depth_image_heights;
    std::vector<std::vector<GLfloat>> depth_images;
    gl::Texture depth_texture = GL_TEXTURE_2D;

    gl::Program cull_program;
    // VAOs have to be compatible with the forward rendering program.
    gl::Program draw_program;
    gl::Program mipmap_program;

    // Shader storage buffer bindings.

    GLuint cull_input_draw_commands_buffer_binding = 0;
    GLuint cull_output_draw_command_count_buffer_binding = 1;
    GLuint cull_output_draw_commands_buffer_binding = 2;
    GLuint cull_world_object_bounds_buffer_binding = 3;

    // Texture units.

    GLuint cull_hzb_texture_unit = 0;

    // Uniform locations.

    GLint cull_world_to_clip_uniform_location = -1;
    GLint cull_hzb_uniform_location = -1;

    GLint draw_model_to_clip_uniform_location = -1;

    GLint mipmap_input_image_uniform_location = -1;
    GLint mipmap_output_image_uniform_location = -1;
};

inline
OcclusionCuller occlusion_culler(eng::ShaderCompiler& sc) {
    auto oc = OcclusionCuller();

    // Programs.

    load(agl::Program(oc.cull_program), sc, {
        {
            agl::compute_shader_tag,
            "iehl/src/occlusion_culling/cull.comp"
        }});
    load(agl::Program(oc.draw_program), sc, {
        {
            agl::vertex_shader_tag,
            "iehl/src/occlusion_culling/draw.vert"
        }, {
            agl::fragment_shader_tag,
            "iehl/src/occlusion_culling/draw.frag"
        }});
    load(agl::Program(oc.mipmap_program), sc, {
        {
            agl::compute_shader_tag,
            "iehl/src/occlusion_culling/mipmap.comp"
        }});

    // Textures.
    
    glTextureStorage2D(oc.depth_texture,
        8, GL_DEPTH_COMPONENT32F, oc.width, oc.height);

    // Framebuffers.

    glNamedFramebufferTexture(oc.depth_fbo,
        GL_DEPTH_ATTACHMENT, oc.depth_texture, 0);

    // Shader storage buffer bindigs.

    gl::ShaderStorageBlockBinding(oc.cull_program,
        "input_draw_commands_buffer",
        oc.cull_input_draw_commands_buffer_binding);
    gl::ShaderStorageBlockBinding(oc.cull_program,
        "output_draw_command_count_buffer",
        oc.cull_output_draw_command_count_buffer_binding);
    gl::ShaderStorageBlockBinding(oc.cull_program,
        "output_draw_commands_buffer",
        oc.cull_output_draw_commands_buffer_binding);
    gl::ShaderStorageBlockBinding(oc.cull_program,
        "world_object_bounds_buffer",
        oc.cull_world_object_bounds_buffer_binding);

    // Uniform locations.

    oc.cull_hzb_uniform_location
    = glGetUniformLocation(oc.cull_program,
        "hzb");
    oc.cull_world_to_clip_uniform_location
    = glGetUniformLocation(oc.cull_program,
        "world_to_clip");

    oc.draw_model_to_clip_uniform_location
    = glGetUniformLocation(oc.draw_program,
        "model_to_clip");

    oc.mipmap_input_image_uniform_location
    = glGetUniformLocation(oc.mipmap_program,
        "input_image");
    oc.mipmap_output_image_uniform_location
    = glGetUniformLocation(oc.mipmap_program,
        "output_image");

    return oc;
}
