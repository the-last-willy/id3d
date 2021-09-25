#pragma once

#include "face_vertex/index/vertex.hpp"
#include "face_vertex/mesh/mesh.hpp"

namespace face_vertex {

struct VertexProxy {
    Mesh& mesh;
    VertexIndex index;

    constexpr
    operator VertexIndex() const noexcept {
        return index;
    }
};

inline
VertexProxy proxy(Mesh& m, VertexIndex vi) {
    return VertexProxy{m, vi};
}

}
