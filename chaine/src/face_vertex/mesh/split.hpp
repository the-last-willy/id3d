#pragma once

#include "face_vertex/index/all.hpp"
#include "face_vertex/proxy/triangle/all.hpp"
#include "face_vertex/proxy/vertex/all.hpp"

namespace face_vertex {

// void split(face_vertex::Mesh& fvmesh) {
//     for(auto triangle : triangles(fvmesh)) {
//         auto p = create_vertex(fvmesh);
//         //position(p) = barycenter(triangle);
//         //std::cout << "(" << position(p)[0] << ", " << position(p)[1] << ", " << position(p)[2] << ")" << std::endl;
//         auto vt2 = vertex(triangle, 2);
//         substitute_vertex(triangle, vertex(triangle, 2), p);
//         //create first triangle
//         auto t0 = create_triangle(fvmesh);
//         topology(t0).vertices[0] = vertex(triangle, 1);
//         topology(t0).vertices[1] = vt2;
//         topology(t0).vertices[2] = p;
//         // //create second triangle
//         auto t1 = create_triangle(fvmesh);
//         topology(t1).vertices[0] = vertex(triangle, 0);
//         topology(t1).vertices[1] = p;
//         topology(t1).vertices[2] = vt2;

//         // //adjacent
//         auto old_adjacent0 = adjacent_triangle(triangle, 0);
//         auto old_adjacent1 = adjacent_triangle(triangle, 1);

//         // //Base triangle
//         substitute_adjacent_triangle(triangle, old_adjacent0, t0);
//         substitute_adjacent_triangle(triangle, old_adjacent1, t1);
//         topology(t0).triangles[0] = triangle;
//         topology(t1).triangles[0] = triangle;
//         //old adjacent
//         substitute_adjacent_triangle(old_adjacent0, triangle, t0);
//         substitute_adjacent_triangle(old_adjacent1, triangle, t1);
//         topology(t0).triangles[1] = old_adjacent0;
//         topology(t1).triangles[2] = old_adjacent1;

//         topology(t0).triangles[2] = t1;
//         topology(t1).triangles[1] = t0;
//     }
// }

VertexProxy split(TriangleProxy triangle) {
        auto p = create_vertex(mesh(triangle));
        auto vt2 = vertex(triangle, 2);
        substitute_vertex(triangle, vertex(triangle, 2), p);
        //Set adjacent triangle of vertex
        topology(p).triangle = triangle;
        topology(vertex(triangle, 2)).triangle = adjacent_triangle(triangle, 0);
        //create first triangle
        auto t0 = create_triangle(mesh(triangle));
        topology(t0).vertices[0] = vertex(triangle, 1);
        topology(t0).vertices[1] = vt2;
        topology(t0).vertices[2] = p;
        // //create second triangle
        auto t1 = create_triangle(mesh(triangle));
        topology(t1).vertices[0] = vertex(triangle, 0);
        topology(t1).vertices[1] = p;
        topology(t1).vertices[2] = vt2;

        // //adjacent
        auto old_adjacent0 = adjacent_triangle(triangle, 0);
        auto old_adjacent1 = adjacent_triangle(triangle, 1);

        // //Base triangle
        substitute_adjacent_triangle(triangle, old_adjacent0, t0);
        substitute_adjacent_triangle(triangle, old_adjacent1, t1);
        topology(t0).triangles[0] = triangle;
        topology(t1).triangles[0] = triangle;
        //old adjacent
        substitute_adjacent_triangle(old_adjacent0, triangle, t0);
        substitute_adjacent_triangle(old_adjacent1, triangle, t1);
        topology(t0).triangles[1] = old_adjacent0;
        topology(t1).triangles[2] = old_adjacent1;

        topology(t0).triangles[2] = t1;
        topology(t1).triangles[1] = t0;

        return p;
}

}