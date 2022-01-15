#pragma once

#include "object/all.hpp"
#include "vertex_attribute/all.hpp"
#include "z_prepasser/all.hpp"

inline
gl::VertexArray vertex_array(
    const VertexAttributeGroup& vag,
    const Z_Prepasser& zp)
{
    auto vao = gl::VertexArray();
    if(gl::GetNamedBufferParameter(vag.position_buffer, GL_BUFFER_SIZE) > 0) {
        auto bindingindex = GLuint(0);
        auto size = GLint(3);
        glVertexArrayAttribBinding(vao,
            zp.world_position,
            bindingindex);
        glVertexArrayAttribFormat(vao,
            zp.world_position,
            size, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayVertexBuffer(vao,
            bindingindex,
            vag.position_buffer,
            0, 4 * size);
        glEnableVertexArrayAttrib(vao,
            zp.world_position);
    }
    return vao;
}

inline
gl::VertexArray vertex_array(
    const ObjectGroup& og,
    const Z_Prepasser& zp)
{
    auto vao = vertex_array(og.vertex_attributes, zp);
    glVertexArrayElementBuffer(vao,
        og.topology.element_buffer);
    return vao;
}
