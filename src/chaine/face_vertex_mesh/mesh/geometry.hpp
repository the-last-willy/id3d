#pragma once

#include <agl/all.hpp>

#include <vector>

namespace chaine::face_vertex_mesh {

struct Geometry {
    std::vector<agl::Vec3> vertex_colors;
    std::vector<float> vertex_mean_curvature;
    std::vector<agl::Vec3> vertex_normals;
    std::vector<agl::Vec3> vertex_positions;
};

inline
void resize_vertices(Geometry& g, std::size_t count) {
    g.vertex_colors.resize(count, agl::vec3(1.f));
    g.vertex_mean_curvature.resize(count, 0.f);
    g.vertex_normals.resize(count);
    g.vertex_positions.resize(count);
}

}
