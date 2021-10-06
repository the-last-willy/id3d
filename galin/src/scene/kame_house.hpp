#pragma once

#include "tree3/all.hpp"

namespace kame_house {

constexpr float pi = 3.14159265359f;

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

inline
SharedNode island() {
    return shared<Object>("island",
        shared<Material>(std::array{1.f, 1.f, 0.f},
            shared<Translation>(std::array{0.f, 0.f, 0.f},
                shared<UniformScaling>(10.f,
                    shared<Ellipsoid>(std::array{1.f, 0.2f, 1.f})
                )
            )
        )
    );
}

inline
SharedNode palm_tree() {
    return shared<Object>("palm_tree",
        shared<Attraction>(5.f,
            shared<Union>(
                shared<Material>(std::array{165.f / 255.f, 42.f / 255.f, 42.f / 255.f},
                    shared<Dilatation>(0.2f,
                        shared<LineSegment>(
                            std::array{2.f, 0.f, 2.f},
                            std::array{5.f, 5.f, 5.f})
                    )
                ),
                shared<Material>(std::array{0.f, 1.f, 0.f},
                    shared<Translation>(std::array{5.f, 5.f, 5.f},
                        shared<Sphere>(2.f)
                    )
                )
            )
        )
    );
}

inline
SharedNode sea() {
    return shared<Object>("sea",
        shared<Material>(std::array{0.f, 0.f, 1.f},
            shared<Swizzling>(std::array{1, 0, 2},
                shared<Plane>(1.f)
            )
        )
    );
}

inline
auto house_walls() {
    return shared<Object>("house_walls",
        shared<Material>(std::array{1.f, 0.f, 0.f},
            shared<Union>(Children{
                shared<Cube>(),
                shared<Translation>(std::array{0.f, 0.5f, 0.f},
                    shared<Scaling>(std::array{1.f / std::sqrt(2.f), 1.f, 1.f},
                        shared<RotatedZ>(pi / 4.f,
                            shared<Cube>()
                        )
                    )
                )
                // ,shared<Translation>(std::array{-0.25f, 0.8f, 0.f},
                //     shared<UniformScaling>(1.f / 4.f,
                //         shared<RotatedY>(pi / 2.f,
                //             shared<Union>(Children{
                //                 shared<Cube>(),
                //                 shared<Translation>(std::array{0.f, 0.5f, 0.f},
                //                     shared<Scaling>(std::array{1.f / std::sqrt(2.f), 1.f, 1.f},
                //                         shared<RotatedZ>(pi / 4.f,
                //                             shared<Cube>()
                //                         )
                //                     )
                //                 )
                //             })
                //         )
                //     )
                // )
            })
        )
    );
}

inline
auto house_roof() {
    return shared<Object>("house_roof",
        shared<Material>(std::array{1.f, 0.f, 0.f},
            shared<Translation>(std::array{0.f, 0.5f, 0.f},
                shared<Scaling>(std::array{1.f / std::sqrt(2.f), 1.f, 1.f},
                    shared<RotatedZ>(pi / 4.f,
                        shared<Cube>()
                    )
                )
            )
        )
    );
}

inline
auto house() {
    return shared<Object>("house",
        house_walls()
    );
}

inline
SharedNode scene() {
    return shared<Object>("scene",
        unionn(
            sea(),
            island(),
            house()
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
