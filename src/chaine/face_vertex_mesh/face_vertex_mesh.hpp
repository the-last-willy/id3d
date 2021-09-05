#pragma once
#include "face.hpp"
#include "vertex.hpp"
#include "triangle_mesh/all.hpp"

#include <vector>
#include <map>

namespace chaine {

struct FaceVertexMesh {
    std::vector<Vertex> vertices = {};
    std::vector<Face> faces = {};
};

struct FacePair {
    unsigned vertex;
    unsigned face; 
};

FaceVertexMesh to_face_vertex_mesh(triangle_mesh::Mesh mesh) {

    FaceVertexMesh fvmesh;
    fvmesh.vertices.resize(mesh.geometry.vertex_positions.size());
    fvmesh.faces.resize(mesh.topology.triangles.size());

    std::map<std::pair<unsigned, unsigned>, FacePair> m;

    for(auto i = 0;  i < mesh.geometry.vertex_positions.size(); ++i) {
        fvmesh.vertices[i].point = mesh.geometry.vertex_positions[i];
    }

    for(auto i = 0; i < mesh.geometry.triangle_indices.size(); ++i) {
        auto indices = fvmesh.faces[i].vertices;
        indices = mesh.geometry.triangle_indices[i];
        fvmesh.vertices[indices[0]].index = i;

        for(auto j = 0; j < 3; ++j) {

            auto max = indices[j] < indices[(j + 1) % 3] ? indices[(j + 1) % 3] : indices[j];
            auto min = indices[j] < indices[(j + 1) % 3] ? indices[j] : indices[(j + 1) % 3];
            
            auto it = m.find(std::pair(min, max));

            if (it != m.end()) {
                auto fp = it->second;
                fvmesh.faces[i].adjacents[(j + 2) % 3] = fp.face;
                fvmesh.faces[fp.face].adjacents[fp.vertex] = i;

                m.erase(it);
            }
            else {
                auto fp = FacePair{.vertex = ((static_cast<unsigned>(j) + 2) % 3), .face = static_cast<unsigned>(i)};
                m.emplace(std::pair(std::pair(min, max), fp));
            }
        }
    }

    return fvmesh;
}
}