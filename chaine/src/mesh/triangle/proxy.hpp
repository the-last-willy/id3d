#pragma once

#include "mesh/index/triangle.hpp"
#include "mesh/mesh/mesh.hpp"

namespace face_vertex {

struct TriangleProxy {
    Mesh* mesh;
    TriangleIndex index;

    constexpr
    operator TriangleIndex() const noexcept {
        return index;
    }
};

inline
TriangleProxy proxy(Mesh& m, TriangleIndex ti) {
    return TriangleProxy{&m, ti};
}

// inline
// bool operator==(TriangleProxy tp0, TriangleProxy tp1) {
//     return tp0.index == tp1.index;
// }

}
