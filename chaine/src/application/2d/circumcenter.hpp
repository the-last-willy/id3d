#pragma once

#include "mesh/triangle/proxy.hpp"
#include "mesh/triangle/vertex.hpp"

#include <agl/opengl/all.hpp>
#include <cmath>

namespace face_vertex {

inline
auto circumcenter(TriangleProxy tp) {
    auto a = position(vertex(tp, 0)).xy();
    auto b = position(vertex(tp, 1)).xy();
    auto c = position(vertex(tp, 2)).xy();
    
    auto ab = b - a;
    auto bc = c - b;
    auto ca = a - c;

    auto tanA = agl::cross(ca, ab) / agl::dot(ca, ab);
    auto tanB = agl::cross(ab, bc) / agl::dot(ab, bc);
    auto tanC = agl::cross(bc, ca) / agl::dot(bc, ca);

    auto wa = tanB + tanC;
    auto wb = tanC + tanA;
    auto wc = tanA + tanB;

    return (wa * a + wb * b + wc * c) / (wa + wb + wc);
}

}