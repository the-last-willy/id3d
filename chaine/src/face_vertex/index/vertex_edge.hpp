#pragma once

#include "vertex.hpp"

#include <array>

namespace face_vertex {

struct EdgeIndexTraits {
    using Value = std::array<VertexIndex, 2>;

    static constexpr Value default_value;
};

using EdgeIndex = Index<EdgeIndexTraits>;

}
