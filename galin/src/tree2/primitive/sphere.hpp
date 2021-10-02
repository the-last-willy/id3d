#pragma once

#include <array>
#include <sstream>

struct Sphere {
    std::array<float, 3> position = {0.f, 0.f, 0.f};
    float radius = 1.f;
};

inline
std::string glsl_expr(const Sphere& p, const std::string& arg) {
    auto ss = std::stringstream();
    ss << "length(" << arg
    << " - vec3(" << p.position[0]
    << ", " << p.position[1]
    << ", " << p.position[2]
    <<  ")) - " << p.radius;
    return ss.str();
}
