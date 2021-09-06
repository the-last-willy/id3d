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
