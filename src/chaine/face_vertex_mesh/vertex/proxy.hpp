#pragma once

#include "chaine/face_vertex_mesh/mesh/mesh.hpp"
#include "index.hpp"

#include <stdexcept>

namespace chaine::face_vertex_mesh {

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
auto& topology(VertexProxy vp) {
    return vp.mesh.topology.vertices[index(vp)];
}

inline
auto& position(VertexProxy vp) {
    return vp.mesh.geometry.vertex_positions[vp.index];
}

////////////////////////////////////////////////////////////////////////////////

inline
auto adjacent_triangles(VertexProxy) {
    throw std::runtime_error("Not implemented.");
}

////////////////////////////////////////////////////////////////////////////////



}
