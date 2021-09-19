#pragma once

#include "generic_index.hpp"

namespace chaine::triangle_mesh {

struct VertexIndexTraits {
    using Value = uint32_t;

    static constexpr Value default_value = 0;
};

using VertexIndex = Index<VertexIndexTraits>;

}
