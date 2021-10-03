#pragma once

#include "face_vertex/all.hpp"

#include <agl/all.hpp>
#include <agl/engine/mesh.hpp>

#include <range/v3/view/enumerate.hpp>

namespace chaine {

inline
eng::Mesh vertex_adjacency_mesh(face_vertex::Mesh& m) {
    auto position_accessor = eng::Accessor();
    {
        auto& accessor = position_accessor;
        {
            auto buffer_ptr = std::make_shared<eng::Buffer>();
            auto& buffer = *buffer_ptr;
            
            buffer.opengl = create(agl::buffer_tag);
            storage(
                buffer.opengl,
                2 * vertex_count(m) * sizeof(agl::Vec3), 
                GL_MAP_WRITE_BIT);
            auto mapping = map<agl::Vec3>(buffer.opengl, GL_WRITE_ONLY);
            for(auto&& [i, v] : ranges::views::enumerate(vertices(m))) {
                mapping[2 * i + 0] = barycenter(any_adjacent_triangle(v));
                mapping[2 * i + 1] = position(v);
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
        primitive.primitive_count = agl::Count<GLsizei>(2 * vertex_count(m));
        primitive.vertex_array = agl::vertex_array();
    }

    return eng::Mesh({std::move(primitive_ptr)});
}

};
