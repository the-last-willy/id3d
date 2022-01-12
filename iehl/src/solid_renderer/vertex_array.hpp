#pragma once

#include "vertex_attribute/all.hpp"
#include "solid_renderer.hpp"

inline
gl::VertexArray vertex_array(const SolidRenderer& sr,
    const VertexAttributeGroup& vag)
{
    auto va = gl::VertexArray();
    if(gl::GetNamedBufferParameter(vag.normal_buffer, GL_BUFFER_SIZE) > 0) {
        auto bindingindex = GLuint(0);
        auto size = GLint(3);
        glVertexArrayAttribBinding(va,
            sr.normal,
            bindingindex);
        glVertexArrayAttribFormat(va,
            sr.normal,
            size, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayVertexBuffer(va,
            bindingindex,
            vag.normal_buffer,
            0, 4 * size);
        glEnableVertexArrayAttrib(va,
            sr.normal);
    }
    if(gl::GetNamedBufferParameter(vag.position_buffer, GL_BUFFER_SIZE) > 0) {
        auto bindingindex = GLuint(1);
        auto size = GLint(3);
        glVertexArrayAttribBinding(va,
            sr.position,
            bindingindex);
        glVertexArrayAttribFormat(va,
            sr.position,
            size, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayVertexBuffer(va,
            bindingindex,
            vag.position_buffer,
            0, 4 * size);
        glEnableVertexArrayAttrib(va,
            sr.position);
    }
    return va;
}
