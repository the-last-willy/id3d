#pragma once

#include "kame_house/roshi.hpp"

namespace kame_house {

template<typename T, typename... Args>
std::shared_ptr<T> shared(Args&&... as) {
    return std::make_shared<T>(std::forward<Args>(as)...);
}

// inline
// auto house() {
//     return shared<Object>(
// "house",
//         Translated(Scaled(Cube(), 5.f), std::array{0.f, 7.f, 0.f}), 
//         Constant(std::array{0.f, 0.f, 0.f}));
// }

// inline
// SharedNode island() {
//     return shared<Object>("island",
//         shared<Material>(std::array{1.f, 1.f, 0.f},
//             shared<Translation>(std::array{0.f, 0.f, 0.f},
//                 shared<UniformScaling>(10.f,
//                     shared<Ellipsoid>(std::array{1.f, 0.2f, 1.f})
//                 )
//             )
//         )
//     );
// }

// inline
// SharedNode palm_tree() {
//     return shared<Object>("palm_tree",
//         shared<Attraction>(5.f,
//             shared<Union>(
//                 shared<Material>(std::array{165.f / 255.f, 42.f / 255.f, 42.f / 255.f},
//                     shared<Dilatation>(0.2f,
//                         shared<LineSegment>(
//                             std::array{2.f, 0.f, 2.f},
//                             std::array{5.f, 5.f, 5.f})
//                     )
//                 ),
//                 shared<Material>(std::array{0.f, 1.f, 0.f},
//                     shared<Translation>(std::array{5.f, 5.f, 5.f},
//                         shared<Sphere>(2.f)
//                     )
//                 )
//             )
//         )
//     );
// }

// inline
// SharedNode sea() {
//     return shared<Object>("sea",
//         shared<Material>(std::array{0.f, 0.f, 1.f},
//             shared<Swizzling>(std::array{1, 0, 2},
//                 shared<Plane>(1.f)
//             )
//         )
//     );
// }

// inline
// auto house_dormer_walls() {
//     return named("house_dormer_walls",
//         material(std::array{1.f, 0.f, 0.f},
//             scaled(1.f, 0.5f, 1.f,
//                 unionn(
//                     cube(),
//                     translated(0.f, 0.5f, 0.f,
//                         scaled(1 / std::sqrt(2.f), 1.f, 1.f,
//                             rotated_z(pi / 4.f,
//                                 cube()
//                             )
//                         )
//                     )
//                 )
//             )
//         )
//     );
// }

// inline
// auto house_doorstep() {
//     return named("house_doorstep",
//         material(std::array{1.f, 1.f, 1.f},
//             unionn(
//                 cube(),
//                 translated(0, 2.76f, 0, rotated_x(0, rotated_y(0, rotated_z(0, scaled(1, 1, 1, 
//                     cube()
//                 ))))),
//                 reflected(0,
//                     translated(0.37f, 1.17f, 0.4f, rotated_x(0, rotated_y(0, rotated_z(0, scaled(0.1f, 2.37f, 0.1f, 
//                         cube()
//                     )))))
//                 )
//             )
//         )
//     );
// }

// inline
// auto house_walls() {
//     return named("house_walls",
//         unionn(
//             material(std::array{1.f, 0.f, 0.f},
//                 scaled(1.f, 0.7f, 1.f,
//                     unionn(
//                         cube(),
//                         translated(0.f, 0.5f, 0.f,
//                             scaled(1 / std::sqrt(2.f), 1.2f, 1.f,
//                                 rotated_z(pi / 4.f,
//                                     cube()
//                                 )
//                             )
//                         )
//                     )
//                 )
//             ),
//             material(std::array{1.f, 1.f, 1.f},
//                 reflected(0,
//                     translated(0.25f, 0.63f, 0, rotated_x(0, rotated_y(0, rotated_z(-0.87f, scaled(0.88f, 0.05f, 1, 
//                         cube()
//                     )))))
//                 )
//             )
//         )
//     );
// }

// inline
// auto house() {
//     return named("house",
//         unionn(
//             house_walls(),
//             translated(0.12f, 0.68f, 0.f, rotated_x(0.f, rotated_y(pi / 2, rotated_z(0.f, scaled(0.26f, 0.45f, 0.24f, 
//                 house_dormer_walls()
//             ))))),
//             translated(-0.25f, -0.27f, 0.65f, rotated_x(0, rotated_y(0, rotated_z(0, scaled(0.5f, 0.16f, 0.32f,
//                 house_doorstep()
//             )))))
//         )
//     );
// }

inline
SharedNode scene() {
    return named("scene",
        unionn(
            roshi()
        )
    );
    // return shared<Object>("scene",
    //     shared<Union>(
    //         shared<Union>(
    //             sea(),
    //             palm_tree()
    //         ),
    //         shared<Union>(
    //             island(),
    //             shared<Translation>(std::array{0.f, 2.f, 0.f},
    //                 house()
    //             )
    //         )
    //     )
    // );
}

}
