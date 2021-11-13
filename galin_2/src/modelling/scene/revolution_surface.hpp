#pragma once

#include "revolution/all.hpp"

#include <agl/engine/all.hpp>

struct RevolutionSurface {
    agl::common::Grid<agl::Vec3> revolution_curve
    = agl::common::Grid<agl::Vec3>(agl::common::grid_indexing(2));

    // agl::Vec3 axis_origin = agl::vec3(0.f);
    // agl::Vec3 axis_direction = agl::vec3(0.f, 1.f, 0.f);

    std::size_t axial_resolution = 2;
    std::size_t radial_resolution = 2;

    //

    std::shared_ptr<agl::engine::TriangleMesh> cpu_tesselation;

    //

    std::shared_ptr<agl::engine::MeshInstance> gpu_control_curve;
    std::shared_ptr<agl::engine::MeshInstance> gpu_tesselation;
};

inline
bool is_valid(const RevolutionSurface& rs) {
    return size(rs.revolution_curve, 0) >= 2;
}

// inline
// auto patch(agl::common::Grid<agl::Vec3> control_points) {
//     auto p = Patch();
//     p.control_points = std::move(control_points);
//     return p;
// }

inline
void tessellate(RevolutionSurface& rs) {
    if(is_valid(rs)) {
        rs.cpu_tesselation = agl::standard::unique(
            revolution_surface(
                rs.revolution_curve,
                agl::mat4(agl::identity),
                uint32_t(rs.axial_resolution),
                uint32_t(rs.radial_resolution)));
    }
}

// inline
// auto bounding_box(Patch& p) {
//     auto bb = agl::common::interval(
//         p.cpu_tesselation->geometry.vertex_positions[0]);
//     for(auto& po : p.cpu_tesselation->geometry.vertex_positions) {
//         extend(bb, po);
//     }
//     return bb;
// }

inline
void transform(RevolutionSurface& rs, const auto& transformation) {
    for(auto& pos : rs.cpu_tesselation->geometry.vertex_positions) {
        pos = transformation(pos);
    }
}

inline
void update_control_mesh(RevolutionSurface& rs) {
    rs.gpu_control_curve = agl::standard::shared(
        agl::engine::instance(
            agl::engine::wireframe(
                control_mesh1(rs.revolution_curve))));
}

inline
void update_gpu(RevolutionSurface& rs) {
    if(is_valid(rs)) {
        rs.gpu_tesselation = agl::standard::shared(
            agl::engine::instance(
                agl::engine::triangle_mesh(
                    *rs.cpu_tesselation, {})));
    }
}

