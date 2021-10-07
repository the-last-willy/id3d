float house_walls_sdf(vec3 position) {
    return sdf_union(
scaling_out(
sdf_cube(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
reflected_x(
position), vec3(0.25, 0.63, 0)), 0), 0), -0.87), vec3(0.88, 0.05, 1))), 0.05),
scaling_out(
sdf_union(
scaling_out(
sdf_cube(
rotated_z(
scaling_in(
translated(
scaling_in(
position, vec3(1, 0.7, 1)), vec3(0, 0.5, 0)), vec3(0.707107, 1.2, 1)), 0.785398)), 0.707107),
sdf_cube(
scaling_in(
position, vec3(1, 0.7, 1)))), 0.7));
}

float house_dormer_walls_sdf(vec3 position) {
    return scaling_out(
sdf_union(
scaling_out(
sdf_cube(
rotated_z(
scaling_in(
translated(
scaling_in(
position, vec3(1, 0.5, 1)), vec3(0, 0.5, 0)), vec3(0.707107, 1, 1)), 0.785398)), 0.707107),
sdf_cube(
scaling_in(
position, vec3(1, 0.5, 1)))), 0.5);
}

float house_doorstep_sdf(vec3 position) {
    return sdf_union(
scaling_out(
sdf_cube(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
reflected_x(
position), vec3(0.37, 1.17, 0.4)), 0), 0), 0), vec3(0.1, 2.37, 0.1))), 0.1),
sdf_union(
scaling_out(
sdf_cube(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(0, 2.76, 0)), 0), 0), 0), vec3(1, 1, 1))), 1),
sdf_cube(
position)));
}

float house_sdf(vec3 position) {
    return sdf_union(
scaling_out(
house_doorstep_sdf(scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(-0.25, -0.27, 0.65)), 0), 0), 0), vec3(0.5, 0.16, 0.32))), 0.16),
sdf_union(
scaling_out(
house_dormer_walls_sdf(scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(0.12, 0.68, 0)), 0), 1.5708), 0), vec3(0.26, 0.45, 0.24))), 0.24),
house_walls_sdf(position)));
}

float scene_sdf(vec3 position) {
    return house_sdf(position);
}

SdfAndMaterial house_walls_material(vec3 position) {
    return sdf_union(
SdfAndMaterial(
scaling_out(
sdf_cube(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
reflected_x(
position), vec3(0.25, 0.63, 0)), 0), 0), -0.87), vec3(0.88, 0.05, 1))), 0.05),
vec3(1, 1, 1)),
SdfAndMaterial(
scaling_out(
sdf_union(
scaling_out(
sdf_cube(
rotated_z(
scaling_in(
translated(
scaling_in(
position, vec3(1, 0.7, 1)), vec3(0, 0.5, 0)), vec3(0.707107, 1.2, 1)), 0.785398)), 0.707107),
sdf_cube(
scaling_in(
position, vec3(1, 0.7, 1)))), 0.7),
vec3(1, 0, 0)));
}

SdfAndMaterial house_dormer_walls_material(vec3 position) {
    return SdfAndMaterial(
scaling_out(
sdf_union(
scaling_out(
sdf_cube(
rotated_z(
scaling_in(
translated(
scaling_in(
position, vec3(1, 0.5, 1)), vec3(0, 0.5, 0)), vec3(0.707107, 1, 1)), 0.785398)), 0.707107),
sdf_cube(
scaling_in(
position, vec3(1, 0.5, 1)))), 0.5),
vec3(1, 0, 0));
}

SdfAndMaterial house_doorstep_material(vec3 position) {
    return SdfAndMaterial(
sdf_union(
scaling_out(
sdf_cube(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
reflected_x(
position), vec3(0.37, 1.17, 0.4)), 0), 0), 0), vec3(0.1, 2.37, 0.1))), 0.1),
sdf_union(
scaling_out(
sdf_cube(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(0, 2.76, 0)), 0), 0), 0), vec3(1, 1, 1))), 1),
sdf_cube(
position))),
vec3(1, 1, 1));
}

SdfAndMaterial house_material(vec3 position) {
    return sdf_union(
scaling_out(
house_doorstep_material(scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(-0.25, -0.27, 0.65)), 0), 0), 0), vec3(0.5, 0.16, 0.32))), 0.16),
sdf_union(
scaling_out(
house_dormer_walls_material(scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(0.12, 0.68, 0)), 0), 1.5708), 0), vec3(0.26, 0.45, 0.24))), 0.24),
house_walls_material(position)));
}

SdfAndMaterial scene_material(vec3 position) {
    return house_material(position);
}

