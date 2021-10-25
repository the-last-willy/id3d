#pragma once

#include "proxy.hpp"

#include <vector>

namespace face_vertex {

inline
bool can_collapse(const TriangleEdgeProxy& tep) {
    auto t0 = adjacent_triangle(tep, 0);
    auto t1 = adjacent_triangle(tep, 1);
    
    auto va = vertex_after(t0, opposite_vertex(t0, t1));
    auto vb = vertex_after(t1, opposite_vertex(t1, t0));

    if(index(va) == index(vb)) {
        return false;
    }

    auto vas = std::vector<VertexIndex>();
    for(auto&& ata : adjacent_triangles(va)) {
        vas.push_back(index(vertex_after(ata, va)));
    }

    auto vbs = std::vector<VertexIndex>();
    for(auto&& atb : adjacent_triangles(vb)) {
        vbs.push_back(index(vertex_after(atb, vb)));
    }

    auto count = uint32_t(0);
    for(auto a : vas) {
        for(auto b : vbs) {
            if(a == b) {
                count += 1;
                break;
            }
        }
    }

    return count == 2;
}

}
