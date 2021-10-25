#pragma once

#include "geometry.hpp"

#include "face_vertex/topology/mesh/topology.hpp"

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
uint32_t vertex_count(const Mesh& m) {
    return static_cast<uint32_t>(size(m.topology.vertices));
}

inline
Geometry& geometry(Mesh& m) {
    return m.geometry;
}

}
