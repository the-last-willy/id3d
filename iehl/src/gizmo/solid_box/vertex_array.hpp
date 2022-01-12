#pragma once

#include "solid_renderer/all.hpp"
#include "object.hpp"

inline
gl::VertexArray vertex_array(
    const SolidBoxVertexAttributes& vas,
    const SolidRenderer& sr)
{
    auto va = gl::VertexArray();

    auto normal_binding = GLuint(0);
    auto position_binding = GLuint(1);

    { // Normal.
        using Normal = SolidBoxVertexAttributes::Normal;
        glVertexArrayAttribFormat(va,
            sr.normal,
            3, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayVertexBuffer(va,
            normal_binding, vas.normal_buffer,
            0, sizeof(Normal));
        glVertexArrayAttribBinding(va,
            sr.normal,
            normal_binding);
        glEnableVertexArrayAttrib(va,
            sr.normal);
    }
    { // Position.
        using Position = SolidBoxVertexAttributes::Position;
        glVertexArrayAttribFormat(va,
            sr.position,
            3, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayVertexBuffer(va,
            position_binding, vas.position_buffer,
            0, sizeof(Position));
        glVertexArrayAttribBinding(va,
            sr.position,
            position_binding);
        glEnableVertexArrayAttrib(va,
            sr.position);
    }

    return va;
}

inline
gl::VertexArray vertex_array(
    const SolidBoxObject& o,
    const SolidRenderer& sr)
{
    auto va = vertex_array(o.vertex_attributes, sr);
    glVertexArrayElementBuffer(va, o.topology.element_buffer);
    return va;
}
