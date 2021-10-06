float house_walls_sdf(vec3 position) {
    return sdf_union(
scaling_out(sdf_union(
scaling_out(sdf_cube(rotated_z(scaling_in(translated(rotated_y(scaling_in(translated(position, vec3(-0.25, 0.8, 0)), 0.25), 1.5708), vec3(0, 0.5, 0)), vec3(0.707107, 1, 1)), 0.785398)), 1),
sdf_cube(rotated_y(scaling_in(translated(position, vec3(-0.25, 0.8, 0)), 0.25), 1.5708))), 0.25),
sdf_union(
scaling_out(sdf_cube(rotated_z(scaling_in(translated(position, vec3(0, 0.5, 0)), vec3(0.707107, 1, 1)), 0.785398)), 1),
sdf_cube(position)));
}

float house_sdf(vec3 position) {
    return house_walls_sdf(position);
}

float scene_sdf(vec3 position) {
    return house_sdf(position);
}

SdfAndMaterial house_walls_material(vec3 position) {
    return SdfAndMaterial(
sdf_union(
scaling_out(sdf_union(
scaling_out(sdf_cube(rotated_z(scaling_in(translated(rotated_y(scaling_in(translated(position, vec3(-0.25, 0.8, 0)), 0.25), 1.5708), vec3(0, 0.5, 0)), vec3(0.707107, 1, 1)), 0.785398)), 1),
sdf_cube(rotated_y(scaling_in(translated(position, vec3(-0.25, 0.8, 0)), 0.25), 1.5708))), 0.25),
sdf_union(
scaling_out(sdf_cube(rotated_z(scaling_in(translated(position, vec3(0, 0.5, 0)), vec3(0.707107, 1, 1)), 0.785398)), 1),
sdf_cube(position))),
vec3(1, 0, 0));
}

SdfAndMaterial house_material(vec3 position) {
    return house_walls_material(position);
}

SdfAndMaterial scene_material(vec3 position) {
    return house_material(position);
}

