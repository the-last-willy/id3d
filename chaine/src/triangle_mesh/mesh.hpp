#pragma once

#include "geometry.hpp"
#include "topology.hpp"

namespace chaine::triangle_mesh {

struct Mesh {
    Geometry geometry;
    Topology topology;
};

inline
uint32_t triangle_count(const Mesh& m) {
    return static_cast<uint32_t>(size(m.topology.triangles));
}

inline
uint32_t vertex_count(const Mesh& m) {
    return static_cast<uint32_t>(size(m.geometry.vertex_positions));
}

}
