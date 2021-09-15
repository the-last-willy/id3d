#pragma once

#include "chaine/index.hpp"

namespace chaine::face_vertex_mesh {

struct TriangleIndexTraits {
    using Value = uint32_t;

    static constexpr Value default_value = 0;
};

using TriangleIndex = Index<TriangleIndexTraits>;

}
