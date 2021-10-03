#pragma once

#include "tree/all.hpp"

namespace kame_house {

// inline
// auto house() {
//     return Object(
//         "house",
//         Translated(Scaled(Cube(), 5.f), std::array{0.f, 7.f, 0.f}), 
//         Constant(std::array{0.f, 0.f, 0.f}));
// }

// inline
// auto island() {
//     auto s = Sphere(50.f);
//     auto ts = Translated(s, std::array{0.f, -45.f, 0.f});
//     return Object("island", ts, Constant(std::array{0.f, 0.f, 0.f}));
// }

// inline
// auto sea() {
//     return Object("sea",
//         Swizzled(Plane(), {0, 2, 1}), 
//         Constant(std::array{0.f, 0.f, 0.f}));
// }

// inline
// auto kame_house() {
//     auto h = house();
//     auto i = island();
//     auto s = sea();
//     auto d = Union(DistanceTo(h), Union(DistanceTo(i), DistanceTo(s)));
//     return Object("scene", d, Constant(std::array{0.f, 0.f, 0.f})); 
// }

inline
auto scene() {
    return Object("scene", Combination(
        Operator(SDF_Union(), ClosestColor()),
        Colored(SDF_Sphere(1.f), {1.f, 0.f, 0.f}),
        Colored(SDF_Sphere(2.f), {0.f, 1.f, 0.f})));
}

}
