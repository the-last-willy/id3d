#pragma once

#include "patch.hpp"
#include "revolution_surface.hpp"

#include <id3d/common/all.hpp>

struct Object {
    // Bezier.

    std::vector<Patch> patches;

    // Revolution.

    RevolutionSurface revolution_surface;

    //

    agl::common::Interval<3> bounding_box;

    // GPU.

    std::shared_ptr<agl::engine::MeshInstance> gpu_bounding_box;
};

inline
void tessellate(Object& o, std::size_t n) {
    for(auto& p : o.patches) {
        tessellate(p, n);
    }
    tessellate(o.revolution_surface);
}

inline
void transform(Object& o, const auto& transformation) {
    for(auto& p : o.patches) {
        transform(p, transformation);
    }
}

inline
void update_bounding_box(Object& o) {
    if(not empty(o.patches)) {
        o.bounding_box = bounding_box(o.patches[0]);
        for(std::size_t i = 1; i < size(o.patches); ++i) {
            o.bounding_box = union_bounds(
                o.bounding_box, bounding_box(o.patches[i]));
        }
    }
}

inline
void update_gpu(Object& o) {
    { // Bounding box.
        o.gpu_bounding_box = agl::standard::shared(
            agl::engine::instance(
                agl::engine::wireframe(
                    gizmo::box_wireframe())));
    }
    for(auto& p : o.patches) {
        update_gpu(p);
    }
    update_gpu(o.revolution_surface);
}
