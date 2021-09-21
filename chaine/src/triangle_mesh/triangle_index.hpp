#pragma once

#include "generic_index.hpp"

namespace triangle_mesh {

struct TriangleIndexTraits {
    using Value = uint32_t;

    static constexpr Value default_value = 0;
};

using TriangleIndex = Index<TriangleIndexTraits>;

}
