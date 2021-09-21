#pragma once

#include "face_vertex/mesh.hpp"
#include "index.hpp"

#include <stdexcept>

namespace face_vertex {

struct VertexProxy {
    Mesh& mesh;
    VertexIndex index;
};

inline
VertexProxy proxy(Mesh& m, VertexIndex vi) {
    return VertexProxy{m, vi};
}

inline
auto index(VertexProxy vp) {
    return vp.index;
}

inline
auto& mesh(VertexProxy vp) {
    return vp.mesh;
}

inline
auto& topology(VertexProxy vp) {
    return vp.mesh.topology.vertices[index(vp)];
}

inline
auto& color(VertexProxy vp) {
    return vp.mesh.geometry.vertex_colors[vp.index];
}

inline
auto& position(VertexProxy vp) {
    return vp.mesh.geometry.vertex_positions[vp.index];
}

inline
auto& normal(VertexProxy vp) {
    return vp.mesh.geometry.vertex_normals[vp.index];
}

////////////////////////////////////////////////////////////////////////////////

// inline
// auto adjacent_triangles(VertexProxy) {
//     throw std::runtime_error("Not implemented.");
// }

////////////////////////////////////////////////////////////////////////////////



}
