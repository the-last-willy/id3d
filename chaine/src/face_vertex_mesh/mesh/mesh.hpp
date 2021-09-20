#pragma once

#include "geometry.hpp"

#include "face_vertex_mesh/topology/mesh.hpp"

namespace chaine::face_vertex_mesh {

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
    return static_cast<uint32_t>(size(m.topology.vertices));
}

}
