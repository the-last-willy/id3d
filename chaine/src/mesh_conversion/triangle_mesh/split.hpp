#include "triangle_mesh/all.hpp"

void split(FaceVertexMesh& fvmesh) {
    auto& topology = fvmesh.topology;
    for(auto triangle : triangles(fvmesh)) {
        auto p = barycenter(triangle);
        topology.vertices.emplace_back(p);
        vertex(triangle, 2) = p;
        //create first triangle
        TriangleTopology t1;
        vertex(t1, 0) = vertex(triangle, 1);
        vertex(t1, 1) = vertex(triangle, 2);
        vertex(t1, 2) = p;
        //create second triangle
        TriangleTopology t2;
        vertex(t2, 0) = vertex(triangle, 0);
        vertex(t2, 1) = p;
        vertex(t2, 2) = vertex(triangle, 2);
        topology.triangles.emplace_back(t1);
        topology.triangles.emplace_back(t2);
    }
}