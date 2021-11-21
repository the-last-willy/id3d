#pragma once

#include <queue>

#include "mesh/all.hpp"

struct SimplificationCandidate {
    SimplificationCandidate(face_vertex::VertexProxy v0, face_vertex::VertexProxy v1)
        : v0(v0)
        , v1(v1)
    {
        distance = face_vertex::distance(v0, v1);
    }

    face_vertex::VertexProxy v0;
    face_vertex::VertexProxy v1;
    float distance = 0.f;
};

inline
bool is_still_valid(const SimplificationCandidate& sc) {
    return is_valid(sc.v0)
    and is_valid(sc.v1)
    and distance(sc.v0, sc.v1) == sc.distance;
}

// inline
// void simplify(face_vertex::Mesh& mesh, std::size_t n) {
//     auto queue = std::priority_queue(
//         [](const SimplificationCandidate& l, const SimplificationCandidate& r) {
//             return l.distance < r.distance;
//         },
//         std::vector<SimplificationCandidate>());
//     for(auto&& t : triangles(mesh)) {
//         for(uint32_t i = 0; i < 3; ++i) {
//             auto v0 = vertex(t, i);
//             auto v1 = vertex(t, i + 1);
//             if(index(v0) < index(v1)) {
//                 queue.push(SimplificationCandidate(v0, v1));
//             }
//         }
//     }
//     for(std::size_t i = 0; i < n; ++i) {

//     }
// }
