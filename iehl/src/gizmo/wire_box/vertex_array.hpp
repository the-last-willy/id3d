#pragma once

#include "wireframe_renderer/all.hpp"
#include "vertex_attributes.hpp"

inline
gl::VertexArray vertex_array(
    const WireBoxVertexAttributes& vas,
    const WireframeRenderer& wr)
{
    using Position = WireBoxVertexAttributes::Position;

    auto va = gl::VertexArray();

    auto position_binding = GLuint(0);

    glVertexArrayAttribFormat(va,
        wr.position_attribute_location,
        3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayVertexBuffer(va,
        position_binding, vas.position_buffer,
        0, sizeof(Position));
    glVertexArrayAttribBinding(va,
        wr.position_attribute_location,
        position_binding);
    glEnableVertexArrayAttrib(va,
        wr.position_attribute_location);

    return va;
}
