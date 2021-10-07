float house_sdf(vec3 position) {
    return sdf_union(
scaling_out(sdf_cube(rotated_z(scaling_in(translated(controlled(position), vec3(0, 0.5, 0)), vec3(0.707107, 1, 1)), 0.785398)), 0.707107),
scaling_out(sdf_cube(scaling_in(controlled(position), vec3(1, 0.5, 1))), 0.5));
}

float scene_sdf(vec3 position) {
    return house_sdf(position);
}

SdfAndMaterial house_material(vec3 position) {
    return SdfAndMaterial(
sdf_union(
scaling_out(sdf_cube(rotated_z(scaling_in(translated(controlled(position), vec3(0, 0.5, 0)), vec3(0.707107, 1, 1)), 0.785398)), 0.707107),
scaling_out(sdf_cube(scaling_in(controlled(position), vec3(1, 0.5, 1))), 0.5)),
vec3(1, 0, 0));
}

SdfAndMaterial scene_material(vec3 position) {
    return house_material(position);
}

