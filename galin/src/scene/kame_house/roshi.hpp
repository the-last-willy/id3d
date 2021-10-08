#pragma once

#include "pi.hpp"

#include "tree3/all.hpp"

namespace kame_house {

constexpr auto roshi_blue_material
= std::array{56.f / 255.f, 52.f / 255.f, 126.f / 255.f};
constexpr auto roshi_orange_material
= std::array{204.f / 255.f, 113.f / 255.f, 61.f / 255.f};
constexpr auto roshi_rod_material
= std::array{91.f / 255.f, 41.f / 255.f, 39.f / 255.f};
constexpr auto roshi_skin_material
= std::array{209.f / 255.f, 143.f / 255.f, 127.f / 255.f};
constexpr auto roshi_white_material
= std::array{222.f / 255.f, 199.f / 255.f, 199.f / 255.f};

inline
SharedNode roshi() {
    return named("roshi",
        unionn(
            material(roshi_orange_material,
                intersection(
                    ellipsoid(0.75f, 2.f, 1.f),
                    inverted(half(1))
                )
            ),
            controlled(
                material(roshi_skin_material,
                    sphere(1.f)
                )
            )
        )
    );
}

}
