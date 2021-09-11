#pragma once

#include "chaine/face_vertex_mesh/mesh/mesh.hpp"

#include "triangle_mesh/all.hpp"

#include <vector>
#include <map>

namespace chaine {

struct FacePair {
    uint32_t vertex;
    face_vertex_mesh::TriangleIndex triangle;
};

inline
face_vertex_mesh::Mesh to_face_vertex_mesh(triangle_mesh::Mesh mesh) {
    face_vertex_mesh::Mesh fvmesh;
    { // Geometry.
        resize_vertices(fvmesh.geometry, vertex_count(mesh));
        fvmesh.geometry.vertex_positions = mesh.geometry.vertex_positions;
    }
    { // Topology.
        fvmesh.topology.vertices.resize(vertex_count(mesh));
        fvmesh.topology.triangles.resize(triangle_count(mesh));
        for(face_vertex_mesh::TriangleIndex i(0); i < triangle_count(mesh); ++i.value) {
            auto& indices = fvmesh.topology.triangles[i].vertices;
            for(auto k = 0; k < 3; ++k) {
                indices[k] = face_vertex_mesh::VertexIndex(mesh.topology.triangles[i].vertices[k]);
            }
        }
    }
    { // Updating face_vertex_mesh topology.
        auto& topology = fvmesh.topology;
        std::map<face_vertex_mesh::EdgeIndex, FacePair> edge_adjacency;
        for(auto triangle : triangles(fvmesh)) {
            auto& indices = face_vertex_mesh::topology(triangle).vertices;
            for(auto v : vertices(triangle)) {
                face_vertex_mesh::topology(v).triangle = index(triangle);
            }
            for(uint32_t j = 0; j < edge_count(triangle); ++j) {
                // Adjacent absolute vertex indices.
                auto i0 = face_vertex_mesh::VertexIndex(indices[j]);
                auto i1 = face_vertex_mesh::VertexIndex(indices[(j + 1) % 3]);
                // RELATIVE INDEX !!!
                auto i2 = (j + 2) % 3;
                // Uniquely identified edge.
                auto edge = face_vertex_mesh::EdgeIndex(
                    i0 < i1 ? std::pair(i0, i1) : std::pair(i1, i0));
                auto it = edge_adjacency.find(edge);
                if (it != edge_adjacency.end()) {
                    // Found an adjacent edge. Connect both ways to it and remove it.
                    auto fp = it->second;
                    face_vertex_mesh::topology(triangle).triangles[i2] = fp.triangle;
                    topology.triangles[fp.triangle].triangles[fp.vertex] = index(triangle);
                    edge_adjacency.erase(it);
                } else {
                    // Did not find an edge. Register current triangle to taht edge.
                    edge_adjacency.emplace(
                        edge, FacePair{.vertex = i2, .triangle = index(triangle)});
                }
            }
        }
    }
    return fvmesh;
}
}