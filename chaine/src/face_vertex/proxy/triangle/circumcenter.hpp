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

    auto tanA = agl::cross(agl::normalize(ca), agl::normalize(ab))[2] / std::abs(agl::dot(ca, ab));
    auto tanB = agl::cross(agl::normalize(ab), agl::normalize(bc))[2] / std::abs(agl::dot(ab, bc));
    auto tanC = agl::cross(agl::normalize(bc), agl::normalize(ca))[2] / std::abs(agl::dot(bc, ca));
    
    std:: cout << "test " << agl::cross(ca, ab)[2] << std:: endl;
    // auto tanA = agl::length(bc) / agl::length(ac);
    // auto tanB = agl::length(ac) / agl::length(bc);
    // auto tanC = agl::length(ba) / agl::length(ca);
    // std::cout << "angle : ";
    // std::cout << std::abs(std::atan(tanA)) + std::abs(std::atan(tanB)) + std::abs(std::atan(tanC)) << std::endl;

    return (tanB + tanC) * a + (tanC + tanA) * b + (tanA + tanB) * c;

    // auto p0 = position(vertex(tp, 0));
    // auto p1 = position(vertex(tp, 1));
    // auto p2 = position(vertex(tp, 2));

    // auto a = agl::length(p1 - p2);
    // auto b = agl::length(p2 - p0);
    // auto c = agl::length(p1 - p0);

    // auto wa = a * (b + c - a);
    // auto wb = b * (c + a - b);
    // auto wc = c * (a + b - c);

    // auto h = (p0 * wa + p1 * wb + p2 * wc) / (wa + wb + wc);

    // std::cout << "p0 = " << agl::length(h - p0) << " p1 = " << agl::length(h - p1) << " p2 = " << agl::length(h - p2) << std::endl;

    // return (p0 * wa + p1 * wb + p2 * wc) / (wa + wb + wc);
}

}