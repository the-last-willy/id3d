float roshi_sdf(vec3 position) {
    return sdf_union(
(sdf_point(controlled(position)) - 1),
sdf_intersection(
inverted(
position.y),
sdf_ellipsoid(
position, vec3(0.75, 2, 1))));
}

float scene_sdf(vec3 position) {
    return roshi_sdf(position);
}

SdfAndMaterial roshi_material(vec3 position) {
    return sdf_union(
SdfAndMaterial(
(sdf_point(controlled(position)) - 1),
vec3(0.819608, 0.560784, 0.498039)),
SdfAndMaterial(
sdf_intersection(
inverted(
position.y),
sdf_ellipsoid(
position, vec3(0.75, 2, 1))),
vec3(0.8, 0.443137, 0.239216)));
}

SdfAndMaterial scene_material(vec3 position) {
    return roshi_material(position);
}

