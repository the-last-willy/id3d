#pragma once

#include "pi.hpp"

#include "tree3/all.hpp"

namespace kame_house {

constexpr auto roshi_blue_material
= std::array{56.f / 255.f, 52.f / 255.f, 126.f / 255.f};
constexpr auto roshi_glasses_frame_material
= std::array{125.f / 255.f, 32.f / 255.f, 19.f / 255.f};
constexpr auto roshi_glasses_material
= std::array{12.f / 255.f, 51.f / 255.f, 17.f / 255.f};
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
            material(roshi_skin_material,
                translated(0.29f, 1.92f, 0, rotated_x(0, rotated_y(0, rotated_z(0, scaled(0.4f, 0.4f, 0.4f,
                    smooth_union(0.2f,
                        sphere(1.f),
                        reflected(2,
                            translated(0, -0.21f, 1.07f, rotated_x(0, rotated_y(-0.93f, rotated_z(0, scaled(0.3f, 0.3f, 0.3f,
                                ellipsoid(1.f, 1.f, 0.5f)
                            )))))
                        ),
                        translated(0.06f, -0.13f, 0, rotated_x(0, rotated_y(0, rotated_z(0, scaled(0.9f, 0.56f, 0.91f,
                            dilated(0.2f,
                                cube()
                            )
                        ))))),
                        translated(0.05f, -0.49f, 0, rotated_x(0, rotated_y(0, rotated_z(0, scaled(1.f, 1.05f, 0.70f,
                            dilated(0.2f,
                                cube()
                            )
                        ))))),
                        scaled(0.1f,
                            dilated(1.f,
                                line_segment(
                                    std::array{10.07f, 0.08f, 0.f},
                                    std::array{10.59f, -4.16f, 0.f}
                                )
                            )
                        ),
                        scaled(0.1f,
                            reflected(2,
                                translated(9.34f, -4.26f, 0.25f, rotated_x(0, rotated_y(0, rotated_z(0, scaled(1.f, 1.f, 1.f,
                                    sphere(1.f)
                                )))))
                            )
                        )
                    )
                )))))
            ),
            material(roshi_white_material,
                translated(0.f, -0.1f, 0.f, scaled(0.1f,
                    dilated(0.1f,
                        unionn(
                            translated(6.46f, 10.32f, -0.14f, rotated_x(0, rotated_y(0, rotated_z(0, scaled(0.83f, 5.2f, 2.0f,
                                    cone(1.f, 1.f)
                            ))))),
                            smooth_union(0.5f,
                                reflected(2,
                                    translated(6.81f, 13.67f, 2.86f, rotated_x(0.64f, rotated_y(0, rotated_z(0, scaled(0.75f, 3.51f, 1.35f,
                                        cone(1.f, 1.f)
                                    )))))
                                ),
                                translated(6.77f, 17.21f, 0, rotated_x(0, rotated_y(0, rotated_z(0, scaled(0.71f, 1.04f, 1.55f,
                                    sphere(1.f)
                                )))))
                            ),
                            reflected(2,
                                difference(
                                    translated(5.52f, 22.04f, 1.29f, rotated_x(0, rotated_y(0, rotated_z(0, scaled(1.19f, 1.24f, 1.21f,
                                        sphere(1.f)
                                    ))))),
                                    translated(7.46f, 20.27f, 1.28f, rotated_x(0, rotated_y(0, rotated_z(0, scaled(2.38f, 4.93f, 1.78f,
                                        sphere(1.f)
                                    )))))
                                )
                            )
                        )
                    )
                ))
            ),
            material(roshi_glasses_frame_material,
                scaled(0.1f,
                    smooth_union(0.1f,
                        reflected(2,
                            translated(7.18f, 19.21f, 1.87f, rotated_x(0, rotated_y(0, rotated_z(0, scaled(0.23f, 1.23f, 1.57f,
                                sphere(1.f)
                            )))))
                        ),
                        translated(7.3f, 19.75f, 0, rotated_x(0, rotated_y(0, rotated_z(0, scaled(0.58f, 2.3f, 1.57f, 
                            dilated(0.1f,
                                line_segment(std::array{0.f, 0.f, -1.f}, std::array{0.f, 0.f, 1.f})
                            )
                        ))))),
                        reflected(2,
                            dilated(0.2f,
                                line_segment(
                                    std::array{2.34f, 19.72f, 4.31f},
                                    std::array{7.08f, 19.52f, 3.26f})
                            )
                        )
                    )
                )
            ),
            material(roshi_glasses_material,
                scaled(0.1f,
                    reflected(2,
                        translated(7.3, 19.17, 1.83, rotated_x(0, rotated_y(0, rotated_z(0, scaled(0.24, 0.97, 1.24, 
                            sphere(1.f)
                        )))))
                    )
                )   
            )
        )
    );
}

}
