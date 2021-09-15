#pragma once

#include "chaine/face_vertex_mesh/all.hpp"
#include "chaine/triangle_mesh/all.hpp"

namespace chaine {

inline
face_vertex_mesh::Mesh to_face_vertex_mesh(triangle_mesh::Mesh tm) {
    face_vertex_mesh::Mesh fvm;
    { // Copy data from 'triangle_mesh' to 'face_vertex_mesh'.
        fvm.geometry.vertex_positions = tm.geometry.vertex_positions;
        fvm.topology.triangles.resize(size(tm.topology.triangles));
        for(std::size_t i = 0; i < size(tm.topology.triangles); ++i) {
            fvm.topology.triangles[i].vertices
            = tm.topology.triangles[i].vertices;
        }
    }
    { // Update topology in 'face_vertex_mesh'.
        // SHOULD BE MOVED IN ITS OWN FUNCTION.

        auto& topology = fvm.topology;

        struct FaceVertex {
            face_vertex_mesh::TriangleIndex triangle;
            uint32_t relative_vertex;
        };

        std::map<face_vertex_mesh::EdgeIndex, FaceVertex> edge_connectivity;

        for(std::size_t i = 0; i < size(topology.triangles); ++i) {
            auto& triangle = topology.triangles
            auto indices = fvmesh.faces[i].vertices;
            indices = mesh.geometry.triangle_indices[i];

            fvmesh.vertices[indices[0]].index = i;

            for(auto j = 0; j < 3; ++j) {
                auto e = unique_edge_index(indices[j], indices[(j + 1) % 3]);
                auto it = edge_connectivity.find(e);
                if(it != end(m)) {
                    auto fv = it->second;
                    fvmesh.faces[i].adjacents[(j + 2) % 3] = fp.face;
                    fvmesh.faces[fp.face].adjacents[fp.vertex] = i;
                    m.erase(it);
                } else {
                    auto fp = FacePair{.vertex = ((static_cast<unsigned>(j) + 2) % 3), .face = static_cast<unsigned>(i)};
                    m.emplace(std::pair(std::pair(min, max), fp));
                }
            }
        }
    }
    return fvm;
}

}
