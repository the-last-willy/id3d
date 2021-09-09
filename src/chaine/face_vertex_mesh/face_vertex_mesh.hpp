#pragma once

#include "mesh/mesh.hpp"

#include "face.hpp"
#include "vertex.hpp"
#include "triangle_mesh/all.hpp"

#include <vector>
#include <map>

namespace chaine {

// struct FaceVertexMesh {
//     std::vector<Vertex> vertices = {};
//     std::vector<Face> faces = {};
// };

struct FacePair {
    face_vertex_mesh::VertexIndex vertex;
    face_vertex_mesh::TriangleIndex face; 
};

inline
face_vertex_mesh::Mesh to_face_vertex_mesh(triangle_mesh::Mesh mesh) {

    face_vertex_mesh::Mesh fvmesh;
    fvmesh.geometry.vertex_positions.resize(mesh.geometry.vertex_positions.size());
    fvmesh.topology.vertices.resize(mesh.geometry.vertex_positions.size());
    fvmesh.topology.triangles.resize(mesh.topology.triangles.size());

    std::map<std::pair<unsigned, unsigned>, FacePair> m;

    for(auto i = 0;  i < mesh.geometry.vertex_positions.size(); ++i) {
        fvmesh.geometry.vertex_positions[i] = mesh.geometry.vertex_positions[i];
    }

    for(auto i = 0; i < mesh.topology.triangles.size(); ++i) {
        auto& indices = fvmesh.topology.triangles[i].vertices;
        for(auto k = 0; k < 3; ++k) {
            indices[k] = face_vertex_mesh::VertexIndex(mesh.topology.triangles[i].vertices[k]);
        }
        fvmesh.topology.vertices[indices[0]].triangle = face_vertex_mesh::TriangleIndex(i);

        for(auto j = 0; j < 3; ++j) {

            auto max = indices[j] < indices[(j + 1) % 3] ? indices[(j + 1) % 3] : indices[j];
            auto min = indices[j] < indices[(j + 1) % 3] ? indices[j] : indices[(j + 1) % 3];
            
            auto it = m.find(std::pair(min, max));

            if (it != m.end()) {
                auto fp = it->second;
                fvmesh.topology.triangles[i].triangles[(j + 2) % 3] = fp.face;
                fvmesh.topology.triangles[fp.face].triangles[fp.vertex] = face_vertex_mesh::TriangleIndex(i);

                m.erase(it);
            }
            else {
                auto fp = FacePair{
                    .vertex = face_vertex_mesh::VertexIndex((static_cast<unsigned>(j) + 2) % 3),
                    .face = face_vertex_mesh::TriangleIndex(static_cast<unsigned>(i))};
                m.emplace(std::pair(std::pair(min, max), fp));
            }
        }
    }

    return fvmesh;
}
}