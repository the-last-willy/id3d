#pragma once

#include "patch.hpp"

struct Object {
    // Definition.

    std::vector<Patch> patchs;

    //

    agl::common::Interval<3> bounding_box;
};

inline
void tesselate(Object& o, std::size_t n) {
    for(auto& p : o.patchs) {
        tesselate(p, n);
    }
}

inline
void transform(Object& o, const auto& transformation) {
    for(auto& p : o.patchs) {
        transform(p, transformation);
    }
}

inline
void update_bounding_box(Object& o) {
    if(not empty(o.patchs)) {
        o.bounding_box = bounding_box(o.patchs[0]);
        for(std::size_t i = 1; i < size(o.patchs); ++i) {
            o.bounding_box = union_bounds(
                o.bounding_box, bounding_box(o.patchs[i]));
        }
    }
}

inline
void update_gpu(Object& o) {
    for(auto& p : o.patchs) {
        update_gpu(p);
    }
}
