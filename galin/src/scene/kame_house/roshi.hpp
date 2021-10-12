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
constexpr auto roshi_purple_material
= std::array{58.f / 255.f, 37.f / 255.f, 67.f / 255.f};
constexpr auto roshi_rod_material
= std::array{91.f / 255.f, 41.f / 255.f, 39.f / 255.f};
constexpr auto roshi_skin_material
= std::array{209.f / 255.f, 143.f / 255.f, 127.f / 255.f};
constexpr auto roshi_white_material
= std::array{222.f / 255.f, 199.f / 255.f, 199.f / 255.f};

inline
SharedNode roshi_rod() {
    return named("roshi_rod",
        material(roshi_rod_material,
            smooth_union(0.2f,
                translated(0.36f, -0.02f, -0.46f, rotated_x(-0, rotated_y(-0, rotated_z(-0, scaled(1.f, 1.08f, 1.f, 
                    dilated(0.05f,
                        line_segment({0.f, -1.f, 0.f}, {0.f, 1.f, 0.f})
                    )
                ))))),
                dilated(0.05f,
                    translated(0.33f, 1.43f, -0.5f, rotated_x(-1.01f, rotated_y(-0.21f, rotated_z(-0.73f, scaled(1.11f, 1.f, 1.55f, 
                        scaled(0.1f,
                            cube()
                        )
                    )))))
                ),
                dilated(0.05f,
                    translated(0.33f, 1.19f, -0.5f, rotated_x(-0.92f, rotated_y(-0.46f, rotated_z(-0, scaled(1.76f, 1.59f, 1.66f, 
                        scaled(0.1f,
                            cube()
                        )
                    )))))
                ),
                dilated(0.05f,
                    translated(0.33f, 1.01f, -0.5f, rotated_x(-1.01f, rotated_y(-0.21f, rotated_z(-0.73f, scaled(0.23f, 0.24f, 0.58f, 
                        scaled(0.1f,
                            cube()
                        )
                    )))))
                )
            )
        )
    );
}

inline
SharedNode roshi_glasses() {
    return named("roshi_glasses",
        unionn(
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
                        translated(7.3f, 19.17f, 1.83f, rotated_x(0, rotated_y(0, rotated_z(0, scaled(0.24f, 0.97f, 1.24f,
                            sphere(1.f)
                        )))))
                    )
                )   
            )
        )    
    );
}

inline
SharedNode roshi_left_arm() {
    return named("roshi_left_arm",
        unionn(
            material(roshi_orange_material,
                intersection(
                    dilated(0.4f,
                        line_segment(
                            std::array{0.f, 1.16f, 0.58f},
                            std::array{0.f, 0.27f, 0.87f}
                        )
                    ),
                    translated(0, 0.2f, 0,
                        inverted(half(1))
                    )
                )
            ),
            material(roshi_skin_material,
                dilated(0.1f,
                    translated(0, 0.2f, 0.92f, rotated_x(-0, rotated_y(-0, rotated_z(-0, scaled(0.125f, 0.13f, 0.15f,
                        cube()
                    )))))
                )
            ),
            material(roshi_white_material,
                translated(0, 0.33f, 0.86f, rotated_x(-0, rotated_y(-0, rotated_z(-0, scaled(0.38f, 0.96f, 0.36f, 
                    dilated(0.2f,
                        circle(1.f)
                    )
                )))))
            ),
            material(roshi_skin_material,
                dilated(0.1f,
                    translated(0, 0.2f, 0.92f, rotated_x(-0, rotated_y(-0, rotated_z(-0, scaled(0.125f, 0.13f, 0.15f,
                        cube()
                    )))))
                )
            )
        )
    );
}

inline
SharedNode roshi_right_arm() {
    return named("roshi_right_arm",
        unionn(
            material(roshi_orange_material,
                unionn(
                    intersection(
                        dilated(0.01f,
                            onion(
                                unionn(
                                    dilated(0.39f,
                                        line_segment({0.f, 1.16f, -0.58f}, {0.f, 0.65f, -0.98f})
                                    ),
                                    dilated(0.39f,
                                        line_segment({0.f, 0.65f, -0.98f}, {0.37f, 0.63f, -0.94f})
                                    )
                                )
                            )
                        ),
                        translated(0.47f, 0, 0, rotated_x(-0, rotated_y(-0, rotated_z(-0, scaled(1.f, 1.f, 1.f, 
                            half(0)
                        )))))
                        
                    )
                )
            ),
            material(roshi_skin_material,
                smooth_union(0.2f,
                    dilated(0.1f,
                        line_segment({0.f, 0.65f, -0.98f}, {0.67f, 0.63f, -0.94f})
                    ),
                    dilated(0.1f,
                        translated(0.73f, 0.61f, -0.9f, rotated_x(-0, rotated_y(-0, rotated_z(-0, scaled(0.125f, 0.13f, 0.15f, 
                            cube()
                        )))))
                    )
                )
            ),
            material(roshi_white_material,
                translated(0.36f, 0.62f, -0.94f, rotated_x(-0, rotated_y(-0, rotated_z(-1.68f, scaled(0.36f, 1.26f, 0.36f, 
                    dilated(0.2f,
                        circle(1.f)
                    )
                )))))
            )
        )
    );
}

inline
SharedNode roshi_lower_half() {
    return named("roshi_lower_half",
        smooth_union(0.2f,
            reflected(2,
                unionn(
                    material(roshi_blue_material,
                        translated(0.01f, -0.88f, 0.24f, rotated_x(-0, rotated_y(-0, rotated_z(-0, scaled(0.49f, 0.94f, 0.53f, 
                            dilated(0.2f,
                                cone(1.f, 1.f)
                            )
                        )))))
                    ),
                    translated(0.17f, -1.25f, 0.3f, rotated_x(-0, rotated_y(0.37f, rotated_z(-0, scaled(0.51f, 0.26f, 0.24f, 
                        intersection(
                            unionn(
                                material(roshi_skin_material,
                                    sphere(1.f)
                                ),
                                material(roshi_purple_material,
                                    translated(-0.08f, 0.12f, 0, rotated_x(-0, rotated_y(-0, rotated_z(-0, scaled(1.29f, 0.74f, 1.21f,
                                        sphere(1.f)
                                    )))))
                                )
                            ),
                            material(roshi_skin_material,
                                inverted(half(1))
                            )
                        )
                    )))))
                )
            ),
            material(roshi_blue_material,
                translated(0.1f, -0.11f, 0.36f, rotated_x(-0, rotated_y(-0, rotated_z(-0, scaled(0.31f, 0.32f, 0.36f,
                    sphere(1.f)
                )))))
            ),
            material(roshi_blue_material,
                translated(-0.03f, -0.26f, -0.31f, rotated_x(-0, rotated_y(-0, rotated_z(-0, scaled(0.331f, 0.32f, 0.36f,
                    sphere(1.f)
                )))))
            )
        )
    );
}

inline
SharedNode roshi_head() {
    return named("roshi_head",
        unionn(
            material(roshi_white_material,
                translated(0.f, -0.1f, 0.f, scaled(0.1f,
                    dilated(0.1f,
                        unionn(
                            translated(6.46f, 10.32f, -0.14f, rotated_x(0, rotated_y(0, rotated_z(0, scaled(0.83f, 5.2f, 2.0f,
                                    cone(1.f, 1.f)
                            ))))),
                            smooth_union(0.3f,
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
            )
        )
    );
}

inline
SharedNode roshi_shirt() {
    return named("roshi_shirt",
        unionn(
            material(roshi_orange_material,
                unionn(
                    smooth_union(0.1f,
                        intersection(
                            ellipsoid(0.65f, 2.f, 1.f),
                            inverted(half(1))
                        ),
                        reflected(2,
                            translated(0, 1.16f, 0.58f, rotated_x(-0, rotated_y(-0, rotated_z(-0, scaled(0.4f, 0.4f, 0.4f,
                                sphere(1.f)
                            )))))
                        )
                    )
                )
            ),
            material(roshi_purple_material,
                scaled(0.1f,
                    unionn(
                        translated(5.73f, 9.51f, 0, rotated_x(-0, rotated_y(-0, rotated_z(-0, scaled(0.62f, 0.39f, 1.f,
                            sphere(1.f)
                        ))))),
                        translated(5.95f, 7.06f, 0, rotated_x(-0, rotated_y(-0, rotated_z(-0, scaled(0.62f, 0.39f, 1.f,
                            sphere(1.f)
                        ))))),
                        translated(6.15f, 4.54f, 0, rotated_x(-0, rotated_y(-0, rotated_z(-0, scaled(0.62f, 0.39f, 1.f,
                            sphere(1.f)
                        ))))),
                        translated(6.2f, 1.88f, 0, rotated_x(-0, rotated_y(-0, rotated_z(-0, scaled(0.86f, 0.34f, 0.94f,
                            sphere(1.f)
                        )))))
                    )
                )
            ),
            material(roshi_white_material,
                intersection(
                    translated(-0.06f, 0, 0, rotated_x(-0, rotated_y(-0, rotated_z(-0, scaled(1.14f, 1.16f, 0.46f, 
                        intersection(
                            ellipsoid(0.65f, 2.f, 1.f),
                            inverted(half(1))
                        )
                    ))))),
                    translated(0.1f, 0.f, 0.f,
                        inverted(half(0))
                    )
                )
            )
        )
    );
}

inline
SharedNode roshi() {
    return named("roshi",
        unionn(
            translated(0.36f, -0.02f, -0.46f, rotated_x(-0, rotated_y(-0, rotated_z(-0, scaled(1.f, 1.08f, 1.f, 
                roshi_rod()
            ))))),
            roshi_shirt(),
            roshi_left_arm(),
            roshi_right_arm(),
            roshi_head(),
            roshi_glasses(),
            roshi_lower_half()
        )
    );
}

}
