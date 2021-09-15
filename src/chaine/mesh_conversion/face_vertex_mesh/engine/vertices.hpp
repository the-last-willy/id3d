#pragma once

#include "chaine/face_vertex_mesh/all.hpp"

#include <agl/engine/all.hpp>

#include <range/v3/view/enumerate.hpp>

namespace chaine {

inline
auto to_vertex_mesh(face_vertex_mesh::Mesh& m) {
    auto position_accessor = eng::Accessor();
    {
        auto& accessor = position_accessor;

        {
            auto buffer_ptr = std::make_shared<eng::Buffer>();
            auto& buffer = *buffer_ptr;
            
            buffer.opengl = create(agl::buffer_tag);
            storage(
                buffer.opengl,
                vertex_count(m) * sizeof(agl::Vec3), 
                GL_MAP_WRITE_BIT);
            auto mapping = map<agl::Vec3>(buffer.opengl, GL_WRITE_ONLY);
            for(auto&& [i, v] : ranges::views::enumerate(vertices(m))) {
                mapping[i] = position(v);
            }
            unmap(buffer.opengl);

            accessor.buffer = buffer_ptr;
        }

        accessor.buffer_view_byte_stride = agl::Stride<GLsizei>(12);
        accessor.component_count = agl::Size<GLint>(3);
        accessor.component_type = GL_FLOAT;
    }
    auto color_accessor = eng::Accessor();
    {
        auto& accessor = color_accessor;

        {
            auto buffer_ptr = std::make_shared<eng::Buffer>();
            auto& buffer = *buffer_ptr;
            
            buffer.opengl = create(agl::buffer_tag);
            storage(
                buffer.opengl,
                vertex_count(m) * sizeof(agl::Vec3), 
                GL_MAP_WRITE_BIT);
            auto mapping = map<agl::Vec3>(buffer.opengl, GL_WRITE_ONLY);
            for(auto&& [i, v] : ranges::views::enumerate(vertices(m))) {
                mapping[i] = color(v);
            }
            unmap(buffer.opengl);

            accessor.buffer = buffer_ptr;
        }

        accessor.buffer_view_byte_stride = agl::Stride<GLsizei>(12);
        accessor.component_count = agl::Size<GLint>(3);
        accessor.component_type = GL_FLOAT;
    }
    auto primitive_ptr = std::make_shared<eng::Primitive>();
    {
        auto& primitive = *primitive_ptr;
        primitive.attributes["COLOR"] = std::move(color_accessor);
        primitive.attributes["POSITION"] = std::move(position_accessor);
        primitive.draw_mode = agl::DrawMode::points;
        primitive.draw_type = agl::DrawType::unsigned_int;
        primitive.primitive_count = agl::Count<GLsizei>(GLsizei(vertex_count(m)));
    }

    return std::make_shared<eng::Mesh>(eng::Mesh({std::move(primitive_ptr)}));
}

}
