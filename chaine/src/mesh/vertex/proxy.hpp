#pragma once

#include "mesh/index/vertex.hpp"
#include "mesh/mesh/mesh.hpp"

namespace face_vertex {

struct VertexProxy {
    Mesh* mesh;
    VertexIndex index;

    constexpr
    operator VertexIndex() const noexcept {
        return index;
    }
};

inline
VertexProxy proxy(Mesh& m, VertexIndex vi) {
    return VertexProxy{&m, vi};
}

}
