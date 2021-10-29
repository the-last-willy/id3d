#pragma once

#include "mesh/all.hpp"
#include "predicate/all.hpp"
#include "contains.hpp"

#include <agl/opengl/all.hpp>

inline
face_vertex::TriangleProxy walk_toward(face_vertex::Mesh& m, agl::Vec2 v) {
    auto t = triangle(m, 0);
    while(true) {
        if(contains(t, v)) {
            return t;
        } else {
            auto v0 = vertex(t, 0);
            auto v1 = vertex(t, 1);
            auto v2 = vertex(t, 2);
            auto p0 = position(v0).xy();
            auto p1 = position(v1).xy();
            auto p2 = position(v2).xy();
            if(orientation(v, p1, p0)) {
                auto ot = opposite_triangle(t, v2);
                if(index(ot) == index(t)) {
                    return t;
                } else {
                    t = ot; 
                }
            }
            if(orientation(v, p2, p1)) {
                auto ot = opposite_triangle(t, v0);
                if(index(ot) == index(t)) {
                    return t;
                } else {
                    t = ot; 
                }
            }
            if(orientation(v, p0, p2)) {
                auto ot = opposite_triangle(t, v1);
                if(index(ot) == index(t)) {
                    return t;
                } else {
                    t = ot; 
                }
            }
        }
    }
}
