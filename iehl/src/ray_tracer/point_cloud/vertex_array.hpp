#pragma once

#include "../ray_tracer.hpp"
#include "point_cloud.hpp"

inline
gl::VertexArray vertex_array(
    const PointCloud& pc,
    const RayTracer& rt)
{
    auto vao = gl::VertexArray();
    if(gl::GetNamedBufferParameter(pc.color_vbo, GL_BUFFER_SIZE) > 0) {
        auto bindingindex = GLuint(0);
        auto size = GLint(3);
        glVertexArrayAttribBinding(vao,
            rt.draw_color,
            bindingindex);
        glVertexArrayAttribFormat(vao,
            rt.draw_color,
            size, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayVertexBuffer(vao,
            bindingindex,
            pc.color_vbo,
            0, sizeof(PointCloud::Color));
        glEnableVertexArrayAttrib(vao,
            rt.draw_color);
    }
    if(gl::GetNamedBufferParameter(pc.normal_vbo, GL_BUFFER_SIZE) > 0) {
        auto bindingindex = GLuint(1);
        auto size = GLint(3);
        glVertexArrayAttribBinding(vao,
            rt.draw_normal,
            bindingindex);
        glVertexArrayAttribFormat(vao,
            rt.draw_normal,
            size, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayVertexBuffer(vao,
            bindingindex,
            pc.normal_vbo,
            0, sizeof(PointCloud::Normal));
        glEnableVertexArrayAttrib(vao,
            rt.draw_normal);
    }
    if(gl::GetNamedBufferParameter(pc.position_vbo, GL_BUFFER_SIZE) > 0) {
        auto bindingindex = GLuint(2);
        auto size = GLint(3);
        glVertexArrayAttribBinding(vao,
            rt.draw_position,
            bindingindex);
        glVertexArrayAttribFormat(vao,
            rt.draw_position,
            size, GL_FLOAT, GL_FALSE, 0);
        glVertexArrayVertexBuffer(vao,
            bindingindex,
            pc.position_vbo,
            0, sizeof(PointCloud::Position));
        glEnableVertexArrayAttrib(vao,
            rt.draw_position);
    }
    return vao;
}
