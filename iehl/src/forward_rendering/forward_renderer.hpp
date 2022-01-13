#pragma once

#include "opengl.hpp"

#include <agl/engine/all.hpp>

struct ForwardRenderer {
    gl::Program program;

    // Attribute locations.

    GLuint normal_attribute_location = 0;
    GLuint position_attribute_location = 1;
    GLuint texcoords_attribute_location = 2;

    // Shader storage buffer bindings.

    GLuint light_culling_index_buffer_binding = 0;
    GLuint light_culling_span_buffer_binding = 1;
    GLuint light_group_light_properties_buffer_binding = 2;
    GLuint material_properties_buffer_binding = 3;
    GLuint material_triangle_material_id_buffer_binding = 4;
    GLuint object_draw_indirect_buffer_binding = 5;

    // Texture units.

    GLuint ao_roughness_metallic_texture_unit = 0;
    GLuint albedo_texture_array_unit = 1;

    // Uniform locations.

    GLint albedo_texture_array_location = 0;
    GLint eye_position_location_location = -1;
    GLint light_culling_domain_lower_bounds_location = -1;
    GLint light_culling_domain_upper_bounds_location = -1;
    GLint light_culling_resolution_location = -1;
    GLint model_to_clip_location = -1;
    GLint model_to_world_location = -1;
    GLint model_to_world_normal_location = -1;
};

inline
auto forward_renderer(eng::ShaderCompiler& sc) {
    auto fr = ForwardRenderer();

    load(agl::Program(fr.program.name), sc, {
        {
            agl::vertex_shader_tag,
            "iehl/src/forward_rendering/shader.vert"
        }, {
            agl::fragment_shader_tag,
            "iehl/src/forward_rendering/shader.frag"
        }});

    // Shader storage buffer bindings.

    gl::ShaderStorageBlockBinding(fr.program,
        "light_culling_index_buffer",
        fr.light_culling_index_buffer_binding);
    gl::ShaderStorageBlockBinding(fr.program,
        "light_culling_span_buffer",
        fr.light_culling_span_buffer_binding);
    gl::ShaderStorageBlockBinding(fr.program,
        "light_group_light_properties_buffer",
        fr.light_group_light_properties_buffer_binding);
    gl::ShaderStorageBlockBinding(fr.program,
        "material_properties_buffer",
        fr.material_properties_buffer_binding);
    gl::ShaderStorageBlockBinding(fr.program,
        "material_triangle_material_id_buffer",
        fr.material_triangle_material_id_buffer_binding);
    gl::ShaderStorageBlockBinding(fr.program,
        "object_draw_indirect_buffer",
        fr.object_draw_indirect_buffer_binding);

    // Uniform locations.

    fr.albedo_texture_array_location
    = glGetUniformLocation(fr.program, "albedo_texture_array");
    fr.eye_position_location_location
    = glGetUniformLocation(fr.program, "eye_world_position");
    fr.light_culling_domain_lower_bounds_location
    = glGetUniformLocation(fr.program, "light_culling_domain_lower_bounds");
    fr.light_culling_domain_upper_bounds_location
    = glGetUniformLocation(fr.program, "light_culling_domain_upper_bounds");
    fr.light_culling_resolution_location
    = glGetUniformLocation(fr.program, "light_culling_resolution");
    fr.model_to_clip_location
    = glGetUniformLocation(fr.program, "model_to_clip");
    fr.model_to_world_location
    = glGetUniformLocation(fr.program, "model_to_world");
    fr.model_to_world_normal_location 
    = glGetUniformLocation(fr.program, "model_to_world_normal");

    return fr;
}
