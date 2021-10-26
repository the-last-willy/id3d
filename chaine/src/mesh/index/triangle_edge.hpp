#pragma once

#include "triangle.hpp"

#include <array>

namespace face_vertex {

struct TriangleEdgeIndexTraits {
    using Value = std::array<TriangleIndex, 2>;

    static constexpr Value default_value = {};
};

using TriangleEdgeIndex = Index<TriangleEdgeIndexTraits>;

}
