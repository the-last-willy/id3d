#pragma once

#include "proxy.hpp"
#include "vertex.hpp"

#include <agl/all.hpp>
#include <cmath>

namespace face_vertex {

inline
auto circumcenter(TriangleProxy tp) {
    auto a = position(vertex(tp, 0));
    auto b = position(vertex(tp, 1));
    auto c = position(vertex(tp, 2));
    
    auto ab = b - a;
    auto bc = c - b;
    auto ca = a - c;

    auto tanA = agl::cross(ca, ab)[2] / agl::dot(ca, ab);
    auto tanB = agl::cross(ab, bc)[2] / agl::dot(ab, bc);
    auto tanC = agl::cross(bc, ca)[2] / agl::dot(bc, ca);

    auto wa = tanB + tanC;
    auto wb = tanC + tanA;
    auto wc = tanA + tanB;

    return (wa * a + wb * b + wc * c) / (wa + wb + wc);
}

}