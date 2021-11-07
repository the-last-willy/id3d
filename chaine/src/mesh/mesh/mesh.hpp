#pragma once

#include "topology/all.hpp"
#include "geometry.hpp"

namespace face_vertex {

struct Mesh {
    Geometry geometry;
    Topology topology;
};

inline
uint32_t triangle_count(const Mesh& m) {
    return static_cast<uint32_t>(size(m.topology.triangles));
}



inline
Geometry& geometry(Mesh& m) {
    return m.geometry;
}

}
