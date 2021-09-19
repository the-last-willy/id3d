#pragma once

#include "face_vertex_mesh/triangle/index.hpp"

#include <array>

namespace chaine::face_vertex_mesh {

struct TriangleEdgeIndexTraits {
    using Value = std::array<TriangleIndex, 2>;

    static constexpr Value default_value = 0;
};

using TriangleEdgeIndex = Index<TriangleEdgeIndexTraits>;

}
