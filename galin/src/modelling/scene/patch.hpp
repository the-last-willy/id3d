#pragma once

#include <agl/engine/all.hpp>

#include <vector>

struct Patch {
    // Definition.

    // [x, y, z].
    agl::common::Grid<agl::Vec3> control_points;

    // Style.

    agl::Vec4 color = agl::vec4(1.f);

    //

    std::shared_ptr<agl::engine::TriangleMesh> cpu_tesselation;

    //

    std::shared_ptr<agl::engine::MeshInstance> gpu_control_mesh;
    std::shared_ptr<agl::engine::MeshInstance> gpu_tesselation;
};

inline
auto patch(agl::common::Grid<agl::Vec3> control_points) {
    auto p = Patch();
    p.control_points = std::move(control_points);
    return p;
}

inline
void tessellate(Patch& p, std::size_t n) {
    p.cpu_tesselation = agl::standard::unique(
        sampled_mesh(
            p.control_points, uint32_t(n), uint32_t(n)));
}

inline
auto bounding_box(Patch& p) {
    auto bb = agl::common::interval(
        p.cpu_tesselation->geometry.vertex_positions[0]);
    for(auto& po : p.cpu_tesselation->geometry.vertex_positions) {
        extend(bb, po);
    }
    return bb;
}

inline
void transform(Patch& p, const auto& transformation) {
    for(auto& pos : p.cpu_tesselation->geometry.vertex_positions) {
        pos = transformation(pos);
    }
}

inline
void update_control_mesh(Patch& p) {
    p.gpu_control_mesh = agl::standard::shared(
        agl::engine::instance(
            agl::engine::wireframe(
                control_mesh(p.control_points))));
}

inline
void update_gpu(Patch& p) {
    p.gpu_tesselation = agl::standard::shared(
        agl::engine::instance(
            agl::engine::triangle_mesh(
                *p.cpu_tesselation, {})));
    p.gpu_tesselation->uniforms["color_factor"]
    = std::make_shared<eng::Uniform<agl::Vec4>>(p.color);
}
