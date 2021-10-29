#pragma once

#include "adjacent_triangle.hpp"
#include "proxy.hpp"

namespace face_vertex {

inline
bool is_valid(TriangleEdgeProxy tep) {
    auto t0 = adjacent_triangle(tep, 0);
    auto t1 = adjacent_triangle(tep, 1);
    if(index(t0) == index(t1)) {
        return false;
    }
    uint32_t i;
    for(i = 0; i < 3; ++i) {
        if(index(adjacent_triangle(t0, i)) == index(t1)) {
            break;
        }
    }
    if(i == 3) {
        return false;
    }
    for(i = 0; i < 3; ++i) {
        if(index(adjacent_triangle(t1, i)) == index(t0)) {
            break;
        }
    }
    return i != 3;;
}

}
