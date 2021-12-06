#pragma once

#include "scene.hpp"

inline
void initialize_gpu(Scene& s) {
    s.vertex_array = agl::create(agl::vertex_array_tag);
    { // Bind element buffer.
        // s.index_buffer = agl::create(agl::buffer_tag);
        // agl::storage(s.index_buffer, std::span(s.triangle_indices));
        element_buffer(s.vertex_array, s.index_buffer);
    }
    if(not empty(s.vertex_normals)) { // Bind vertex normals.
        s.normal_buffer = agl::create(agl::buffer_tag);
        agl::storage(s.normal_buffer, std::span(s.vertex_normals));
        attribute_binding(
            s.vertex_array,
            agl::AttributeIndex<GLint>(0),
            agl::BindingIndex<GLuint>(0));
        attribute_format(
            s.vertex_array,
            agl::AttributeIndex<GLint>(0),
            agl::Size<GLint>(3),
            GL_FLOAT,
            agl::Normalized(false),
            agl::Offset<GLuint>(0));
        vertex_buffer(
            s.vertex_array,
            agl::BindingIndex<GLuint>(0),
            s.normal_buffer,
            agl::Offset<GLintptr>(0),
            agl::Stride<GLsizei>(12));
        enable(
            s.vertex_array,
            agl::AttributeIndex<GLint>(0));
    }
    { // Bind vertex positions.
        s.position_buffer = agl::create(agl::buffer_tag);
        agl::storage(s.position_buffer, std::span(s.vertex_positions));
        attribute_binding(
            s.vertex_array,
            agl::AttributeIndex<GLint>(1),
            agl::BindingIndex<GLuint>(1));
        attribute_format(
            s.vertex_array,
            agl::AttributeIndex<GLint>(1),
            agl::Size<GLint>(3),
            GL_FLOAT,
            agl::Normalized(false),
            agl::Offset<GLuint>(0));
        vertex_buffer(
            s.vertex_array,
            agl::BindingIndex<GLuint>(1),
            s.position_buffer,
            agl::Offset<GLintptr>(0),
            agl::Stride<GLsizei>(12));
        enable(
            s.vertex_array,
            agl::AttributeIndex<GLint>(1));
    }
    // Bind vertex texcoords.
    if(not empty(s.vertex_texcoords)) {
        s.texcoord_buffer = agl::create(agl::buffer_tag);
        storage(s.texcoord_buffer, std::span(s.vertex_texcoords));
        attribute_binding(
            s.vertex_array,
            agl::AttributeIndex<GLint>(2),
            agl::BindingIndex<GLuint>(2));
        attribute_format(
            s.vertex_array,
            agl::AttributeIndex<GLint>(2),
            agl::Size<GLint>(2),
            GL_FLOAT,
            agl::Normalized(false),
            agl::Offset<GLuint>(0));
        vertex_buffer(
            s.vertex_array,
            agl::BindingIndex<GLuint>(2),
            s.texcoord_buffer,
            agl::Offset<GLintptr>(0),
            agl::Stride<GLsizei>(8));
        enable(
            s.vertex_array,
            agl::AttributeIndex<GLint>(2));
    }
    { // Material ssbo.
        s.material_ssbo = agl::create(agl::buffer_tag);
        storage(s.material_ssbo, std::span(s.materials));
    }
}
