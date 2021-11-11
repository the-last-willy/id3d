#pragma once

#include <agl/engine/all.hpp>

namespace gizmo {

#pragma once

inline
agl::engine::TriangleMesh box_wireframe() {
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
        auto f = &topology(create_face(tm, 2));
        f->incident_vertices[0] = index(v000);
        f->incident_vertices[1] = index(v001);
        f = &topology(create_face(tm, 2));
        f->incident_vertices[0] = index(v000);
        f->incident_vertices[1] = index(v010);
        f = &topology(create_face(tm, 2));
        f->incident_vertices[0] = index(v000);
        f->incident_vertices[1] = index(v100);
        f = &topology(create_face(tm, 2));
        f->incident_vertices[0] = index(v001);
        f->incident_vertices[1] = index(v011);
        f = &topology(create_face(tm, 2));
        f->incident_vertices[0] = index(v001);
        f->incident_vertices[1] = index(v101);
        f = &topology(create_face(tm, 2));
        f->incident_vertices[0] = index(v010);
        f->incident_vertices[1] = index(v011);
        f = &topology(create_face(tm, 2));
        f->incident_vertices[0] = index(v010);
        f->incident_vertices[1] = index(v110);
        f = &topology(create_face(tm, 2));
        f->incident_vertices[0] = index(v011);
        f->incident_vertices[1] = index(v111);
        f = &topology(create_face(tm, 2));
        f->incident_vertices[0] = index(v100);
        f->incident_vertices[1] = index(v101);
        f = &topology(create_face(tm, 2));
        f->incident_vertices[0] = index(v100);
        f->incident_vertices[1] = index(v110);
        f = &topology(create_face(tm, 2));
        f->incident_vertices[0] = index(v101);
        f->incident_vertices[1] = index(v111);
        f = &topology(create_face(tm, 2));
        f->incident_vertices[0] = index(v110);
        f->incident_vertices[1] = index(v111);
    }
    return tm;
}

inline
agl::Mat4 box_wireframe_model_to_world(const agl::engine::BoundingBox& bb) {
    return inverse(mat4(transform(bb)))
    * agl::translation(midpoint(bb.aabb))
    * agl::scaling3(length(bb.aabb));
}

}
