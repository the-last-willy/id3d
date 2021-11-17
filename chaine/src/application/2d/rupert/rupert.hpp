#pragma once

#include "settings.hpp"

#include <agl/constant/all.hpp>

inline
void rupert(const RupertSettings&, face_vertex::Mesh& m) {
    for(auto&& t : triangles(m)) {
        auto sa = smallest_angle(t) * agl::constant::degree_per_radian;
        std::cout << "a=" << sa << std::endl;
    }
}
