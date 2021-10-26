#pragma once

#include "proxy.hpp"
#include "relative_index.hpp"
#include "vertex.hpp"

#include "mesh/vertex/index.hpp"

#include <stdexcept>

namespace face_vertex {

inline
void substitute_vertex(
    TriangleProxy tp,
    VertexIndex before,
    VertexIndex after)
{
    topology(tp)->vertices[relative_index(tp, before)] = after;
}

}
