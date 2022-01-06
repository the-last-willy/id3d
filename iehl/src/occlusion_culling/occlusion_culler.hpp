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

    // VAOs have to be compatible with the forward rendering program.
    gl::Program draw_program;
    gl::Program test_program;

    // Draw indirect commands.

    std::vector<gl::DrawElementsIndirectCommand> draw_indirect_commands;
    gl::Buffer draw_indirect_buffer;

    GLsizei draw_count = 0;
    gl::Buffer draw_count_buffer;

    // Uniform locations.

    GLint draw_model_to_clip_uniform_location = -1;
};

inline
OcclusionCuller occlusion_culler(eng::ShaderCompiler& sc) {
    auto oc = OcclusionCuller();

    // Programs.

    load(agl::Program(oc.draw_program), sc, {
        {
            agl::vertex_shader_tag,
            "iehl/src/occlusion_culling/draw.vert"
        }, {
            agl::fragment_shader_tag,
            "iehl/src/occlusion_culling/draw.frag"
        }});

    // load(agl::Program(oc.test_program), sc, {
    //     {
    //         agl::vertex_shader_tag,
    //         "iehl/src/occlusion_culling/test.vert"
    //     }, {
    //         agl::fragment_shader_tag,
    //         "iehl/src/occlusion_culling/test.frag"
    //     }});

    // Textures.
    
    glTextureStorage2D(oc.depth_texture,
        8, GL_DEPTH_COMPONENT32F, oc.width, oc.height);

    // Framebuffers.

    glNamedFramebufferTexture(oc.depth_fbo,
        GL_DEPTH_ATTACHMENT, oc.depth_texture, 0);

    // Uniform locations.

    oc.draw_model_to_clip_uniform_location
    = glGetUniformLocation(oc.draw_program,
        "model_to_clip");

    return oc;
}
