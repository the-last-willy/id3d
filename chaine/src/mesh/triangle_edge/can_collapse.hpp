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

    if(index(opposite_vertex(t0, t1)) == index(opposite_vertex(t1, t0))) {
        return false;
    }

    auto vas = std::vector<VertexIndex>();
    // std::cout << "va" << index(va) << ": ";
    for(auto&& ata : adjacent_triangles(va)) {
        vas.push_back(index(vertex_after(ata, va)));
        // std::cout << index(vertex_after(ata, va)) << " ";
    }
    // std::cout << std::endl;

    auto vbs = std::vector<VertexIndex>();
    // std::cout << "vb" << index(vb) << ": ";
    for(auto&& atb : adjacent_triangles(vb)) {
        vbs.push_back(index(vertex_after(atb, vb)));
        // std::cout << index(vertex_after(atb, vb)) << " ";
    }
    // std::cout << std::endl;

    auto count = uint32_t(0);
    for(auto a : vas) {
        for(auto b : vbs) {
            if(a == b) {
                count += 1;
                break;
            }
        }
    }

    // std::cout << count << std::endl;

    return count == 2;
}

}
