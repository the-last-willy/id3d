#pragma once

#include "is_valid.hpp"
#include "proxy.hpp"
#include "topology.hpp"

#include <sstream>

namespace face_vertex {

inline
std::string string(TriangleProxy tp) {
    auto ss = std::stringstream();
    ss << "triangle " << index(tp);
    if(not is_valid(tp)) {
        ss << " (invalid)"; 
    }
    ss << "\n";
    ss << "  adjacent T: "
        << topology(tp).triangles[0] << ", "
        << topology(tp).triangles[1] << ", "
        << topology(tp).triangles[2] << "\n";
    ss << "  incident V: "
        << topology(tp).vertices[0] << ", "
        << topology(tp).vertices[1] << ", "
        << topology(tp).vertices[2] << "\n";
    return ss.str();
}

}
