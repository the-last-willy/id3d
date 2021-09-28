#pragma once

#include <agl/engine/all.hpp>

namespace gizmo {

inline
eng::Material segment_material() {
    auto m = eng::Material();
    
    return m;   
}

inline
eng::Primitive segment_primitive() {
    auto p = eng::Primitive();

    return p;
}

inline
eng::Mesh segment() {
    auto me = eng::Mesh();
    auto& p = *(me.primitives.emplace_back(std::make_shared<eng::Primitive>()));
    return me;
}

struct Gizmo {
    Gizmo() {

    }
};



}
