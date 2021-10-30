#pragma once

#include "triangle/adjacent_triangle.hpp"
#include "triangle/has_incident_vertex.hpp"
#include "triangle/is_adjacent_to.hpp"
#include "triangle/is_valid.hpp"
#include "vertex/any_incident_triangle.hpp"
#include "vertex/is_valid.hpp"
#include "topology.hpp"
#include "triangle.hpp"
#include "vertex.hpp"

namespace face_vertex {

inline
bool is_valid(Topology& to) {
    for(uint32_t i = 0; i < size(to.triangles); ++i) {
        auto&& tr = proxy(to, TriangleIndex(i));
        if(is_valid(tr)) {
            for(uint32_t j = 0; j < 3; ++j) {
                auto&& at = adjacent_triangle(tr, j);
                if(index(at) != index(tr)) {
                    if(not is_valid(tr)) {
                        return false;
                    }
                    if(not is_adjacent_to(tr, index(at))) {
                        return false;
                    }
                }
            }
        }
    }
    for(uint32_t i = 0; i < size(to.vertices); ++i) {
        auto&& v = proxy(to, VertexIndex(i));
        if(is_valid(v)) {
            if(not has_incident_vertex(any_incident_triangle(v), index(v))) {
                return false;
            }
        }
    }
    return true;
}

}