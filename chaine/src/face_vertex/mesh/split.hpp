#pragma once

#include "face_vertex/index/all.hpp"
#include "face_vertex/proxy/triangle/all.hpp"

namespace face_vertex {

void split(face_vertex::Mesh& fvmesh) {
    auto& topology = fvmesh.topology;
    for(auto triangle : triangles(fvmesh)) {
        auto p = barycenter(triangle);
        topology.vertices.emplace_back(p);
        auto i2 = index(vertex(triangle, 2));
        auto p_index = TriangleIndex(topology.vertices.size() - 1);
        topology(triangle).vertices[2] = p_index;
        //create first triangle
        TriangleTopology t1;
        t1.vertices[0] = topology.vertices[index(vertex(triangle, 1))];
        t1.vertices[1] = topology.vertices[i2];
        t1.vertices[2] = topology.vertices[p_index];
        //create second triangle
        TriangleTopology t2;
        t2.vertices[0] = topology.vertices[index(vertex(triangle, 0))];
        t2.vertices[1] = topology.vertices[p_index];
        t2.vertices[2] = topology.vertices[i2];

        topology.triangles.emplace_back(t1);
        topology.triangles.emplace_back(t2);

        //adjacent
        //give triangle index not triangle topology
        auto t1_index = topology.triangles.size() - 2;
        auto t2_index = topology.triangles.size() - 1;

        auto old_adjacent1 = topology(triangle).triangles[vertex(triangles(1))];
        auto old_adjacent2 = topology(triangle).triangles[vertex(triangles(2))];

        //Base triangle
        substitue_adjacent_triangle(triangle, old_adjacent1, t1_index);
        substitue_adjacent_triangle(triangle, old_adjacent2, t2_index);
        face_vertex::topology(t1).triangles[vertex(triangle(1))] = index(triangle);
        face_vertex::topology(t2).triangles[vertex(triangle(2))] = index(triangle);
        //old adjacent
        substitue_adjacent_triangle(topology.triangles[old_adjacent1], triangle, t1_index);
        substitue_adjacent_triangle(topology.triangles[old_adjacent2], triangle, t2_index);
        ace_vertex::topology(t1).triangles[vertex(triangle(1))] = old_adjacent1;
        face_vertex::topology(t2).triangles[vertex(triangle(2))] = old_adjacent2;

        face_vertex::topology(t1).triangles[vertex(triangle(0))] = t2_index;
        face_vertex::topology(t2).triangles[vertex(triangle(0))] = t1_index;

        // face_vertex::topology(t1).triangles[vertex(triangle(2))] = face_vertex::topology(triangle).triangles[vertex(triangle(0))];
        // face_vertex::topology(t1).triangles[vertex(triangle(1))] = face_vertex::topology(triangle).triangles[vertex(triangle(1))];

        // face_vertex::topology(triangle).triangles[vertex(triangle(0))] = t1_index;
        // face_vertex::topology(t1).triangles[vertex(triangle(1))] = index(triangle);

        // face_vertex::topology(triangle).triangles[vertex(triangle(1))] = t2_index;
        // face_vertex::topology(t2).triangles[vertex(triangle(2))] = index(triangle);
    }
}

}