#pragma once

#include "proxy.hpp"
#include "topology.hpp"

namespace face_vertex {

inline
bool is_valid(TriangleProxy tp) {
    return is_valid(topology(tp));
}

}
