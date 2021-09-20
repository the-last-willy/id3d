#pragma once

#include "index.hpp"

#include "face_vertex/mesh.hpp"

namespace face_vertex {

struct TriangleProxy {
    Mesh* mesh;
    TriangleIndex index;
};

inline
TriangleProxy proxy(Mesh& m, TriangleIndex ti) {
    return TriangleProxy{&m, ti};
}

}
