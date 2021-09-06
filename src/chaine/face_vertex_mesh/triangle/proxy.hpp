#pragma once

#include "chaine/face_vertex_mesh/mesh/mesh.hpp"
#include "index.hpp"

namespace chaine::face_vertex_mesh {

struct TriangleProxy {
    Mesh& mesh;
    TriangleIndex index;
};

inline
TriangleProxy proxy(Mesh& m, TriangleIndex ti) {
    return TriangleProxy{m, ti};
}

inline
auto vertices(TriangleProxy) {
    throw std::runtime_error("Not implemented.");
}

}
