#pragma once

#include "face_vertex/all.hpp"
#include "triangle_mesh/all.hpp"

#include <vector>
#include <map>

namespace chaine {

struct FacePair {
    uint32_t vertex;
    face_vertex::TriangleIndex triangle;
};

inline
face_vertex::Mesh to_face_vertex_mesh(triangle_mesh::Mesh mesh) {
    face_vertex::Mesh fvmesh;
    { // Geometry.
        resize_vertices(fvmesh.geometry, vertex_count(mesh));
        fvmesh.geometry.vertex_positions = mesh.geometry.vertex_positions;
    }
    { // Topology.
        fvmesh.topology.vertices.resize(vertex_count(mesh));
        fvmesh.topology.triangles.resize(triangle_count(mesh));
        for(face_vertex::TriangleIndex i(0); i < triangle_count(mesh); ++i.value) {
            auto& indices = fvmesh.topology.triangles[i].vertices;
            for(auto k = 0; k < 3; ++k) {
                indices[k] = face_vertex::VertexIndex(mesh.topology.triangles[i].vertices[k]);
            }
        }
    }
    { // Updating face_vertex_mesh topology.
        auto& topology = fvmesh.topology;
        std::map<face_vertex::EdgeIndex, FacePair> edge_adjacency;
        for(auto triangle : triangles(fvmesh)) {
            auto& indices = face_vertex::topology(triangle).vertices;
            for(auto v : vertices(triangle)) {
                face_vertex::topology(v).triangle = index(triangle);
            }
            for(uint32_t j = 0; j < edge_count(triangle); ++j) {
                // Adjacent absolute vertex indices.
                auto i0 = face_vertex::VertexIndex(indices[j]);
                auto i1 = face_vertex::VertexIndex(indices[(j + 1) % 3]);
                // RELATIVE INDEX !!!
                auto i2 = (j + 2) % 3;
                // Uniquely identified edge.
                auto edge = face_vertex::EdgeIndex(
                    i0 < i1 ? std::array{i0, i1} : std::array{i1, i0});
                auto it = edge_adjacency.find(edge);
                if (it != edge_adjacency.end()) {
                    // Found an adjacent edge. Connect both ways to it and remove it.
                    auto fp = it->second;
                    face_vertex::topology(triangle).triangles[i2] = fp.triangle;
                    topology.triangles[fp.triangle].triangles[fp.vertex] = index(triangle);
                    edge_adjacency.erase(it);
                } else {
                    // Did not find an edge. Register current triangle to taht edge.
                    edge_adjacency.emplace(
                        edge, FacePair{.vertex = i2, .triangle = index(triangle)});
                }
            }
        }
        // // Ghost topology.
        // if(not empty(edge_adjacency)) {
        //     { // Make ghost vertex.
        //         // We don't care about its geometry. Should it be removed ?
        //         resize_vertices(fvmesh.geometry, vertex_count(mesh) + 1);
        //         fvmesh.topology.vertices.resize(vertex_count(mesh) + 1); 
        //         fvmesh.topology.vertices.back().is_ghost = true;
        //     }
        //     { // Make ghost triangle with remaining edges.
        //         fvmesh.topology.triangles.reserve(triangle_count(mesh) + size(edge_adjacency));
        //         auto ghost_vertex = VertexIndex(vertex_count(mesh) - 1);
        //         for(auto e : edge_adjacency) {
        //             auto& t = fvmesh.topology.triangles.emplace_back();
        //             t.triangles[0] = e.second.triangle;
        //             t.triangles[1] = ;
        //             t.triangles[2] = ;
        //             t.vertices[0] = ghost_vertex;
        //             t.vertices[1] = ;
        //             t.vertices[2] = ;
        //         }
        //     }
        // }
    }
    return fvmesh;
}

}
