float sea_sdf(vec3 position) {
    return (sdf_plane(position.yxz) - 0.5);
}

float island_sdf(vec3 position) {
    return scaling_out(sdf_ellipsoid(scaling_in(translated(position, vec3(0, 0, 0)), 10), vec3(1, 0.2, 1)), 10);
}

float house_walls_sdf(vec3 position) {
    return sdf_union(
scaling_out(sdf_cube(rotated_z(scaling_in(translated(position, vec3(0, 0.5, 0)), vec3(0.707107, 1, 1)), 0.785398)), 1),
sdf_cube(position));
}

float house_sdf(vec3 position) {
    return house_walls_sdf(position);
}

float scene_sdf(vec3 position) {
    return sdf_union(
house_sdf(position),
sdf_union(
island_sdf(position),
sea_sdf(position)));
}

SdfAndMaterial sea_material(vec3 position) {
    return SdfAndMaterial(
(sdf_plane(position.yxz) - 0.5),
vec3(0, 0, 1));
}

SdfAndMaterial island_material(vec3 position) {
    return SdfAndMaterial(
scaling_out(sdf_ellipsoid(scaling_in(translated(position, vec3(0, 0, 0)), 10), vec3(1, 0.2, 1)), 10),
vec3(1, 1, 0));
}

SdfAndMaterial house_walls_material(vec3 position) {
    return SdfAndMaterial(
sdf_union(
scaling_out(sdf_cube(rotated_z(scaling_in(translated(position, vec3(0, 0.5, 0)), vec3(0.707107, 1, 1)), 0.785398)), 1),
sdf_cube(position)),
vec3(1, 0, 0));
}

SdfAndMaterial house_material(vec3 position) {
    return house_walls_material(position);
}

SdfAndMaterial scene_material(vec3 position) {
    return sdf_union(
house_material(position),
sdf_union(
island_material(position),
sea_material(position)));
}

