#pragma once

#include "mesh/all.hpp"
#include "find_border_triangle.hpp"
#include "paraboloid.hpp"
#include "walk_toward.hpp"

#include <agl/opengl/all.hpp>

inline
void insert(face_vertex::Mesh& m, agl::Vec2 v) {
    auto dst = walk_toward(m, v);
    is_valid(topology(m));
    if(contains(dst, v)) {
        // Interior.
        auto s = split(dst);
        is_valid(topology(m));
        position(s) = paraboloid(v);
    } else {
        // Exterior.
        auto v0 = vertex(dst, 0);
        auto v1 = vertex(dst, 1);
        auto v2 = vertex(dst, 2);
        auto p0 = position(v0).xy();
        auto p1 = position(v1).xy();
        auto p2 = position(v2).xy();

        auto vert = create_vertex(m);
        auto tr = create_triangle(m);

        topology(vert)->triangle = tr;
        position(vert) = paraboloid(v);
        

        topology(tr)->triangles[0] = index(dst);
        topology(tr)->vertices[0] = index(vert);

        auto e0 = face_vertex::VertexProxy();
        auto e1 = face_vertex::VertexProxy();

        if(orientation(v, p1, p0)) {
            topology(dst)->triangles[2] = index(tr);
            topology(tr)->vertices[1] = index(v1);
            topology(tr)->vertices[2] = index(v0);
            e0 = v0;
            e1 = v1;
        } else if(orientation(v, p2, p1)) {
            topology(dst)->triangles[0] = index(tr);
            topology(tr)->vertices[1] = index(v2);
            topology(tr)->vertices[2] = index(v1);
            e0 = v1;
            e1 = v2;
        } else if(orientation(v, p0, p2)) {
            topology(dst)->triangles[1] = index(tr);
            topology(tr)->vertices[1] = index(v0);
            topology(tr)->vertices[2] = index(v2);
            e0 = v2;
            e1 = v0;
        }

        { // CCW
            auto prev_tr = tr;
            auto prev_e = e1;
            while(true) {
                auto next_tr = find_border_triangle_ccw(prev_e, prev_tr);
                auto next_e = vertex(next_tr, relative_index(next_tr, prev_e) + 1);
                if(orientation(v, position(next_e).xy(), position(prev_e).xy())) {
                    auto new_tr = create_triangle(m);
                    
                    topology(new_tr)->vertices[0] = index(vert);
                    topology(new_tr)->vertices[1] = index(next_e);
                    topology(new_tr)->vertices[2] = index(prev_e);

                    topology(new_tr)->triangles[0] = index(next_tr);
                    topology(new_tr)->triangles[1] = index(prev_tr);
                    
                    topology(next_tr)->triangles[(relative_index(next_tr, next_e) + 1) % 3] = index(new_tr);
                    topology(prev_tr)->triangles[(relative_index(prev_tr, prev_e) + 1) % 3] = index(new_tr);

                    prev_tr = new_tr;
                    prev_e = next_e;
                } else {
                    break;
                }
            }
        }

        { // CW
            auto prev_tr = tr;
            auto prev_e = e0;
            while(true) {
                auto next_tr = find_border_triangle_cw(prev_e, prev_tr);
                auto next_e = vertex(next_tr, relative_index(next_tr, prev_e) + 2);
                if(orientation(v, position(prev_e).xy(), position(next_e).xy())) {
                    auto new_tr = create_triangle(m);
                    
                    topology(new_tr)->vertices[0] = index(vert);
                    topology(new_tr)->vertices[1] = index(prev_e);
                    topology(new_tr)->vertices[2] = index(next_e);

                    topology(new_tr)->triangles[0] = index(next_tr);
                    topology(new_tr)->triangles[2] = index(prev_tr);
                    
                    topology(next_tr)->triangles[(relative_index(next_tr, next_e) + 2) % 3] = index(new_tr);
                    topology(prev_tr)->triangles[(relative_index(prev_tr, prev_e) + 2) % 3] = index(new_tr);

                    prev_tr = new_tr;
                    prev_e = next_e;
                } else {
                    break;
                }
            }
        }
    }
}
