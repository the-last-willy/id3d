#pragma once

#include "triangle.hpp"
#include "vertex.hpp"

#include <tuple>

namespace face_vertex {

struct TriangleVertexEdgeIndexTraits {
    using Value = std::tuple<TriangleIndex, VertexIndex>;

    static constexpr Value default_value = {};
};

using TriangleVertexEdgeIndex = Index<TriangleVertexEdgeIndexTraits>;

}
