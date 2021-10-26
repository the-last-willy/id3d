#pragma once

#include "proxy.hpp"

namespace face_vertex {

inline
VertexIndex index(VertexTopologyProxy vtp) {
    return vtp.index;
}

}
