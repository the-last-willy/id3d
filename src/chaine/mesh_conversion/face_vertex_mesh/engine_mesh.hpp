#pragma once

#include "chaine/face_vertex_mesh/all.hpp"

#include <agl/all.hpp>
#include <agl/engine/mesh.hpp>

#include <range/v3/view/enumerate.hpp>

namespace chaine {

inline
eng::Mesh triangle_adjacency_mesh(face_vertex_mesh::Mesh& m) {
    auto index_accessor = eng::Accessor();
    {
        auto& accessor = index_accessor;
        {
            auto buffer_ptr = std::make_shared<eng::Buffer>();
            auto& buffer = *buffer_ptr;

            buffer.opengl = create(agl::buffer_tag);
            storage(
                buffer.opengl,
                2 * 3 * triangle_count(m) * sizeof(uint32_t), 
                GL_MAP_WRITE_BIT);
            auto mapping = map<uint32_t>(buffer.opengl, GL_WRITE_ONLY);
            for(auto&& [i, tr] : ranges::views::enumerate(triangles(m))) {
                mapping[6 * i + 0] = tr.index;
                mapping[6 * i + 1] = adjacent_triangle(tr, 0).index;
                mapping[6 * i + 2] = tr.index;
                mapping[6 * i + 3] = adjacent_triangle(tr, 1).index;
                mapping[6 * i + 4] = tr.index;
                mapping[6 * i + 5] = adjacent_triangle(tr, 2).index;
            }
            unmap(buffer.opengl);

            accessor.buffer = buffer_ptr;
        }
        accessor.buffer_view_byte_stride = agl::Stride<GLsizei>(4);
        accessor.component_count = agl::Size<GLint>(1);
        accessor.component_type = GL_UNSIGNED_INT;
        
    }
    auto position_accessor = eng::Accessor();
    {
        auto& accessor = position_accessor;
        {
            auto buffer_ptr = std::make_shared<eng::Buffer>();
            auto& buffer = *buffer_ptr;
            
            buffer.opengl = create(agl::buffer_tag);
            storage(
                buffer.opengl,
                triangle_count(m) * sizeof(agl::Vec3), 
                GL_MAP_WRITE_BIT);
            auto mapping = map<agl::Vec3>(buffer.opengl, GL_WRITE_ONLY);
            for(auto&& [i, tr] : ranges::views::enumerate(triangles(m))) {
                mapping[i] = barycenter(tr);
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
        primitive.primitive_count = agl::Count<GLsizei>(2 * 3 * triangle_count(m));
        primitive.vertex_array = agl::vertex_array();
    }

    return eng::Mesh({std::move(primitive_ptr)});
}

};
