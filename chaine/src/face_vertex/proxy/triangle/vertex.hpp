#pragma once

#include "index.hpp"
#include "mesh.hpp"
#include "proxy.hpp"
#include "topology.hpp"
#include "vertex_count.hpp"

#include "face_vertex/proxy/vertex/proxy.hpp"

#include <agl/opengl/all.hpp>

namespace face_vertex {

inline
auto vertex(TriangleProxy tp, uint32_t i) {
    return proxy(
        mesh(tp),
        topology(tp).vertices[i % vertex_count(tp)]);
}

}
