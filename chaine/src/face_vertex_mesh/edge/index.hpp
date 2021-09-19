#pragma once

#include "face_vertex_mesh/vertex/index.hpp"

#include <utility>

namespace chaine::face_vertex_mesh {

struct EdgeIndexTraits {
    using Value = std::pair<VertexIndex, VertexIndex>;

    static constexpr Value default_value;
};

using EdgeIndex = Index<EdgeIndexTraits>;

inline
EdgeIndex unique_edge_index(VertexIndex vi0, VertexIndex vi1) {
    if(vi0 < vi1) {
        return EdgeIndex(vi0, vi1);
    } else {
        return EdgeIndex(vi1, vi0);
    }
}

}
