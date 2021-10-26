#pragma once

#include "mesh.hpp"
#include "topology.hpp"

namespace face_vertex {

inline
uint32_t vertex_count(Mesh& m) {
    return vertex_count(topology(m));
}

}
