#pragma once

#include "scene/vertex_attribute/all.hpp"
#include "forward_renderer.hpp"

inline
gl::VertexArray vertex_array(
    const ForwardRenderer&,
    const VertexAttributeGroup& vag)
{
    auto vao = gl::VertexArray();
    if(gl::GetNamedBufferParameter(vag.normal_buffer, GL_BUFFER_SIZE) > 0) {
        auto attribindex = GLuint(0);
        auto bindingindex = GLuint(0);
        auto size = GLint(3);
        glVertexArrayAttribBinding(vao,
            attribindex, bindingindex);
        glVertexArrayAttribFormat(vao,
            attribindex, size, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayVertexBuffer(vao,
            bindingindex, vag.normal_buffer, 0, 4 * size);
        glEnableVertexArrayAttrib(vao, attribindex);
    }
    if(gl::GetNamedBufferParameter(vag.position_buffer, GL_BUFFER_SIZE) > 0) {
        auto attribindex = GLuint(1);
        auto bindingindex = GLuint(1);
        auto size = GLint(3);
        glVertexArrayAttribBinding(vao,
            attribindex, bindingindex);
        glVertexArrayAttribFormat(vao,
            attribindex, size, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayVertexBuffer(vao,
            bindingindex, vag.position_buffer, 0, 4 * size);
        glEnableVertexArrayAttrib(vao, attribindex);
    }
    if(gl::GetNamedBufferParameter(vag.texcoords_buffer, GL_BUFFER_SIZE) > 0) {
        auto attribindex = GLuint(2);
        auto bindingindex = GLuint(2);
        auto size = GLint(2);
        glVertexArrayAttribBinding(vao,
            attribindex, bindingindex);
        glVertexArrayAttribFormat(vao,
            attribindex, size, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayVertexBuffer(vao,
            bindingindex, vag.texcoords_buffer, 0, 4 * size);
        glEnableVertexArrayAttrib(vao, attribindex);
    }
    return vao;
}
