#pragma once

#include "mesh.hpp"
#include "proxy.hpp"

#include "face_vertex/proxy/triangle/proxy.hpp"

namespace face_vertex {

inline
TriangleProxy any_adjacent_triangle(VertexProxy vp) {
    return proxy(mesh(vp), topology(vp).triangle);
}

}
