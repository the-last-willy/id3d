#pragma once

#include "mesh/all.hpp"

inline
face_vertex::TriangleProxy find_border_triangle_ccw(
    face_vertex::VertexProxy pivot,
    face_vertex::TriangleProxy start)
{
    while(true) {
        auto next = adjacent_triangle(start, relative_index(start, pivot) + 2);
        if(index(next) == index(start)) {
            return start;
        } else {
            start = next;
        }
    }
}

inline
face_vertex::TriangleProxy find_border_triangle_cw(
    face_vertex::VertexProxy pivot,
    face_vertex::TriangleProxy start)
{
    while(true) {
        auto next = adjacent_triangle(start, relative_index(start, pivot) + 1);
        if(index(next) == index(start)) {
            return start;
        } else {
            start = next;
        }
    }
}
