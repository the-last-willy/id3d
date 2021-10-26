#pragma once

#include "proxy.hpp"

namespace face_vertex {

inline
TriangleIndex index(TriangleTopologyProxy ttp) {
    return ttp.index;
}

}
