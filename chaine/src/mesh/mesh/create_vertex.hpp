#pragma once

#include "mesh.hpp"
#include "topology.hpp"

#include "mesh/vertex/proxy.hpp"

namespace face_vertex {

inline
VertexProxy create_vertex(Mesh& m) {
    geometry(m).vertex_colors.emplace_back();
    geometry(m).vertex_mean_curvature.emplace_back();
    geometry(m).vertex_normals.emplace_back();
    geometry(m).vertex_positions.emplace_back();
    return proxy(m, index(create_vertex(topology(m))));
}

}

