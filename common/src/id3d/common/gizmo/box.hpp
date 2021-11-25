#pragma once

#include <agl/engine/all.hpp>

namespace gizmo {

#pragma once

inline
agl::engine::TriangleMesh box() {
    auto tm = agl::engine::TriangleMesh();
    auto v000 = create_vertex(tm);
    position(v000) = agl::vec3(-0.5f, -0.5f, -0.5f);
    auto v001 = create_vertex(tm);
    position(v001) = agl::vec3(-0.5f, -0.5f, 0.5f);
    auto v010 = create_vertex(tm);
    position(v010) = agl::vec3(-0.5f, 0.5f, -0.5f);
    auto v011 = create_vertex(tm);
    position(v011) = agl::vec3(-0.5f, 0.5f, 0.5f);
    auto v100 = create_vertex(tm);
    position(v100) = agl::vec3(0.5f, -0.5f, -0.5f);
    auto v101 = create_vertex(tm);
    position(v101) = agl::vec3(0.5f, -0.5f, 0.5f);
    auto v110 = create_vertex(tm);
    position(v110) = agl::vec3(0.5f, 0.5f, -0.5f);
    auto v111 = create_vertex(tm);
    position(v111) = agl::vec3(0.5f, 0.5f, 0.5f);
    {
        auto f = &topology(create_face(tm, 3));
        f->incident_vertices[] = index();
        f->incident_vertices[] = index();
        f->incident_vertices[] = index();
        auto f = &topology(create_face(tm, 3));
        f->incident_vertices[] = index();
        f->incident_vertices[] = index();
        f->incident_vertices[] = index();
        auto f = &topology(create_face(tm, 3));
        f->incident_vertices[] = index();
        f->incident_vertices[] = index();
        f->incident_vertices[] = index();
        auto f = &topology(create_face(tm, 3));
        f->incident_vertices[] = index();
        f->incident_vertices[] = index();
        f->incident_vertices[] = index();
        auto f = &topology(create_face(tm, 3));
        f->incident_vertices[] = index();
        f->incident_vertices[] = index();
        f->incident_vertices[] = index();
        auto f = &topology(create_face(tm, 3));
        f->incident_vertices[] = index();
        f->incident_vertices[] = index();
        f->incident_vertices[] = index();
        auto f = &topology(create_face(tm, 3));
        f->incident_vertices[] = index();
        f->incident_vertices[] = index();
        f->incident_vertices[] = index();
        auto f = &topology(create_face(tm, 3));
        f->incident_vertices[] = index();
        f->incident_vertices[] = index();
        f->incident_vertices[] = index();
        auto f = &topology(create_face(tm, 3));
        f->incident_vertices[] = index();
        f->incident_vertices[] = index();
        f->incident_vertices[] = index();
        auto f = &topology(create_face(tm, 3));
        f->incident_vertices[] = index();
        f->incident_vertices[] = index();
        f->incident_vertices[] = index();
        auto f = &topology(create_face(tm, 3));
        f->incident_vertices[] = index();
        f->incident_vertices[] = index();
        f->incident_vertices[] = index();
        auto f = &topology(create_face(tm, 3));
        f->incident_vertices[] = index();
        f->incident_vertices[] = index();
        f->incident_vertices[] = index();
    }
    return tm;
}

inline
agl::Mat4 box_model_to_world(const agl::common::Interval<agl::Vec3>& i) {
    return agl::translation(midpoint(i))
    * agl::scaling3(length(i));
}

inline
agl::Mat4 box_model_to_world(const agl::engine::BoundingBox& bb) {
    return inverse(mat4(transform(bb)))
    * agl::translation(midpoint(bb.aabb))
    * agl::scaling3(length(bb.aabb));
}

}
