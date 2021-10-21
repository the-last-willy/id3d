#pragma once

#include "mesh.hpp"
#include "triangle_proxy.hpp"
#include "vertex_proxy.hpp"

#include <range/v3/view/enumerate.hpp>

namespace triangle_mesh {

inline
auto triangle_mesh(const Mesh& m) {
    auto index_accessor = std::make_shared<eng::Accessor>();
    {
        auto& accessor = *index_accessor;

        {
            auto buffer_ptr = std::make_shared<eng::Buffer>();
            auto& buffer = *buffer_ptr;

            buffer.opengl = create(agl::buffer_tag);
            storage(
                buffer.opengl,
                3 * triangle_count(m) * sizeof(uint32_t), 
                GL_MAP_WRITE_BIT);
            auto mapping = map<std::array<VertexIndex, 3>>(buffer.opengl, GL_WRITE_ONLY);
            for(auto&& [i, t] : ranges::views::enumerate(triangles(m))) {
                mapping[i] = topology(t).vertices;
            }
            unmap(buffer.opengl);

            accessor.buffer = buffer_ptr;
        }

        accessor.buffer_view_byte_stride = agl::Stride<GLsizei>(4);
        accessor.component_count = agl::Size<GLint>(1);
        accessor.component_type = GL_UNSIGNED_INT;
        
    }
    auto position_accessor = std::make_shared<eng::Accessor>();
    {
        auto& accessor = *position_accessor;

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
    
    auto primitive_ptr = std::make_shared<eng::Primitive>();
    {
        auto& primitive = *primitive_ptr;
        primitive.attributes["POSITION"] = std::move(position_accessor);
        primitive.draw_type = agl::DrawType::unsigned_int;
        primitive.indices = std::move(index_accessor);
        primitive.primitive_count = agl::Count<GLsizei>(GLsizei(3 * triangle_count(m)));
    }

    return std::make_shared<eng::Mesh>(eng::Mesh({std::move(primitive_ptr)}));
}

inline
auto edge_mesh(const Mesh& m) {
    auto index_accessor = std::make_shared<eng::Accessor>();
    {
        auto& accessor = *index_accessor;

        {
            auto buffer_ptr = std::make_shared<eng::Buffer>();
            auto& buffer = *buffer_ptr;

            buffer.opengl = create(agl::buffer_tag);
            storage(
                buffer.opengl,
                6 * triangle_count(m) * sizeof(uint32_t), 
                GL_MAP_WRITE_BIT);
            auto mapping = map<uint32_t>(buffer.opengl, GL_WRITE_ONLY);
            for(auto&& [i, tr] : ranges::views::enumerate(triangles(m))) {
                auto& to = topology(tr);
                mapping[6 * i + 0] = to.vertices[0];
                mapping[6 * i + 1] = to.vertices[1];
                mapping[6 * i + 2] = to.vertices[1];
                mapping[6 * i + 3] = to.vertices[2];
                mapping[6 * i + 4] = to.vertices[2];
                mapping[6 * i + 5] = to.vertices[0];
            }
            unmap(buffer.opengl);

            accessor.buffer = buffer_ptr;
        }

        accessor.buffer_view_byte_stride = agl::Stride<GLsizei>(4);
        accessor.component_count = agl::Size<GLint>(1);
        accessor.component_type = GL_UNSIGNED_INT;
        
    }
    auto position_accessor = std::make_shared<eng::Accessor>();
    {
        auto& accessor = *position_accessor;

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
    
    auto primitive_ptr = std::make_shared<eng::Primitive>();
    {
        auto& primitive = *primitive_ptr;
        primitive.attributes["POSITION"] = std::move(position_accessor);
        primitive.draw_mode = agl::DrawMode::lines;
        primitive.draw_type = agl::DrawType::unsigned_int;
        primitive.indices = std::move(index_accessor);
        primitive.primitive_count = agl::Count<GLsizei>(6 * triangle_count(m));
        primitive.vertex_array = agl::vertex_array();
    }

    return std::make_shared<eng::Mesh>(eng::Mesh({std::move(primitive_ptr)}));
}


inline
auto vertex_mesh(const Mesh& m) {
    auto position_accessor = std::make_shared<eng::Accessor>();
    {
        auto& accessor = *position_accessor;

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
    
    auto primitive_ptr = std::make_shared<eng::Primitive>();
    {
        auto& primitive = *primitive_ptr;
        primitive.attributes["POSITION"] = std::move(position_accessor);
        primitive.draw_mode = agl::DrawMode::points;
        primitive.draw_type = agl::DrawType::unsigned_int;
        primitive.primitive_count = agl::Count<GLsizei>(GLsizei(vertex_count(m)));
    }

    return std::make_shared<eng::Mesh>(eng::Mesh({std::move(primitive_ptr)}));
}

}
