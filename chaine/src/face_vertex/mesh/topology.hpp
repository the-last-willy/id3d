#pragma once

#include "mesh.hpp"

namespace face_vertex {

inline
const Topology& topology(const Mesh& m) {
    return m.topology;
}

inline
Topology& topology(Mesh& m) {
    return m.topology;
}

}
