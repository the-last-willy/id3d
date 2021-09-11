#pragma once

#include "proxy.hpp"

#include "chaine/face_vertex_mesh/triangle/proxy.hpp"

namespace chaine::face_vertex_mesh {

inline
TriangleProxy any_adjacent_triangle(VertexProxy vp) {
    return proxy(mesh(vp), topology(vp).triangle);
}

}
