float roshi_rod_sdf(vec3 position) {
    return smooth_union(0.2,
dilated(0.05,
corrected(0.23,
corrected(0.1,
cube(
scaled(vec3(0.1, 0.1, 0.1),
scaled(vec3(0.23, 0.24, 0.58),
rotated_z(-0.73,
rotated_y(-0.21,
rotated_x(-1.01,
translated(vec3(0.33, 1.01, -0.5),
position)))))))))),
smooth_union(0.2,
dilated(0.05,
corrected(1.59,
corrected(0.1,
cube(
scaled(vec3(0.1, 0.1, 0.1),
scaled(vec3(1.76, 1.59, 1.66),
rotated_y(-0.46,
rotated_x(-0.92,
translated(vec3(0.33, 1.19, -0.5),
position))))))))),
smooth_union(0.2,
dilated(0.05,
corrected(1.,
corrected(0.1,
cube(
scaled(vec3(0.1, 0.1, 0.1),
scaled(vec3(1.11, 1, 1.55),
rotated_z(-0.73,
rotated_y(-0.21,
rotated_x(-1.01,
translated(vec3(0.33, 1.43, -0.5),
position)))))))))),
corrected(1.,
dilated(0.05,
line_segment(vec3(0, -1, 0), vec3(0, 1, 0), scaled(vec3(1, 1.08, 1),
translated(vec3(0.36, -0.02, -0.46),
position))))))));
}

float roshi_sdf(vec3 position) {
    return roshi_rod_sdf(
position);
}

float scene_sdf(vec3 position) {
    return roshi_sdf(
position);
}

SdfAndMaterial roshi_rod_material(vec3 position) {
    return SdfAndMaterial(vec3(0.356863, 0.160784, 0.152941),
smooth_union(0.2,
dilated(0.05,
corrected(0.23,
corrected(0.1,
cube(
scaled(vec3(0.1, 0.1, 0.1),
scaled(vec3(0.23, 0.24, 0.58),
rotated_z(-0.73,
rotated_y(-0.21,
rotated_x(-1.01,
translated(vec3(0.33, 1.01, -0.5),
position)))))))))),
smooth_union(0.2,
dilated(0.05,
corrected(1.59,
corrected(0.1,
cube(
scaled(vec3(0.1, 0.1, 0.1),
scaled(vec3(1.76, 1.59, 1.66),
rotated_y(-0.46,
rotated_x(-0.92,
translated(vec3(0.33, 1.19, -0.5),
position))))))))),
smooth_union(0.2,
dilated(0.05,
corrected(1.,
corrected(0.1,
cube(
scaled(vec3(0.1, 0.1, 0.1),
scaled(vec3(1.11, 1, 1.55),
rotated_z(-0.73,
rotated_y(-0.21,
rotated_x(-1.01,
translated(vec3(0.33, 1.43, -0.5),
position)))))))))),
corrected(1.,
dilated(0.05,
line_segment(vec3(0, -1, 0), vec3(0, 1, 0), scaled(vec3(1, 1.08, 1),
translated(vec3(0.36, -0.02, -0.46),
position)))))))));
}

SdfAndMaterial roshi_material(vec3 position) {
    return roshi_rod_material(
position);
}

SdfAndMaterial scene_material(vec3 position) {
    return roshi_material(
position);
}

