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

    auto tanA = agl::cross(ca, ab)[2] / std::abs(agl::dot(ca, ab));
    auto tanB = agl::cross(ab, bc)[2] / std::abs(agl::dot(ab, bc));
    auto tanC = agl::cross(bc, ca)[2] / std::abs(agl::dot(bc, ca));
    
    // std::cout << "angle : ";
    // std::cout << std::abs(std::atan(tanA)) + std::abs(std::atan(tanB)) + std::abs(std::atan(tanC)) << std::endl;

    auto wa = tanB + tanC;
    auto wb = tanC + tanA;
    auto wc = tanA + tanB;

    auto h = (a * wa + b * wb + c * wc) / (wa + wb + wc);

    std::cout << "a = " << agl::length(h - a) << " b = " << agl::length(h - b) << " c = " << agl::length(h - c) << std::endl;   

    return (wa * a + wb * b + wc * c) / (wa + wb + wc);
}

}