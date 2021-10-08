float roshi_sdf(vec3 position) {
    return sdf_union(
scaling_out(
scaling_out(
(sdf_point(scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
reflected_z(
scaling_in(
position, vec3(0.1, 0.1, 0.1))), vec3(7.3, 19.17, 1.83)), 0), 0), 0), vec3(0.24, 0.97, 1.24))) - 1), 0.24), 0.1),
sdf_union(
scaling_out(
smooth_union(
dilated(sdf_line_segment(
vec3(2.34, 19.72, 4.31),
vec3(7.08, 19.52, 3.26),
reflected_z(
scaling_in(
position, vec3(0.1, 0.1, 0.1)))), 0.2),
smooth_union(
scaling_out(
dilated(sdf_line_segment(
vec3(0, 0, -1),
vec3(0, 0, 1),
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
scaling_in(
position, vec3(0.1, 0.1, 0.1)), vec3(7.3, 19.75, 0)), 0), 0), 0), vec3(0.58, 2.3, 1.57))), 0.1), 0.58),
scaling_out(
(sdf_point(scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
reflected_z(
scaling_in(
position, vec3(0.1, 0.1, 0.1))), vec3(7.18, 19.21, 1.87)), 0), 0), 0), vec3(0.23, 1.23, 1.57))) - 1), 0.23),
0.1),
0.1), 0.1),
sdf_union(
scaling_out(
dilated(sdf_union(
difference(scaling_out(
(sdf_point(scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
reflected_z(
scaling_in(
translated(
position, vec3(0, -0.1, 0)), vec3(0.1, 0.1, 0.1))), vec3(5.52, 22.04, 1.29)), 0), 0), 0), vec3(1.19, 1.24, 1.21))) - 1), 1.19), scaling_out(
(sdf_point(scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
reflected_z(
scaling_in(
translated(
position, vec3(0, -0.1, 0)), vec3(0.1, 0.1, 0.1))), vec3(7.46, 20.27, 1.28)), 0), 0), 0), vec3(2.38, 4.93, 1.78))) - 1), 1.78)),
sdf_union(
smooth_union(
scaling_out(
(sdf_point(scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
scaling_in(
translated(
position, vec3(0, -0.1, 0)), vec3(0.1, 0.1, 0.1)), vec3(6.77, 17.21, 0)), 0), 0), 0), vec3(0.71, 1.04, 1.55))) - 1), 0.71),
scaling_out(
cone_sdf(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
reflected_z(
scaling_in(
translated(
position, vec3(0, -0.1, 0)), vec3(0.1, 0.1, 0.1))), vec3(6.81, 13.67, 2.86)), 0.64), 0), 0), vec3(0.75, 3.51, 1.35)),
vec2(1,1)
), 0.75),
0.5),
scaling_out(
cone_sdf(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
scaling_in(
translated(
position, vec3(0, -0.1, 0)), vec3(0.1, 0.1, 0.1)), vec3(6.46, 10.32, -0.14)), 0), 0), 0), vec3(0.83, 5.2, 2)),
vec2(1,1)
), 0.83))), 0.1), 0.1),
sdf_union(
scaling_out(
smooth_union(
scaling_out(
scaling_out(
(sdf_point(scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
reflected_z(
scaling_in(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(0.29, 1.92, 0)), 0), 0), 0), vec3(0.4, 0.4, 0.4)), vec3(0.1, 0.1, 0.1))), vec3(9.34, -4.26, 0.25)), 0), 0), 0), vec3(1, 1, 1))) - 1), 1), 0.1),
smooth_union(
scaling_out(
dilated(sdf_line_segment(
vec3(10.07, 0.08, 0),
vec3(10.59, -4.16, 0),
scaling_in(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(0.29, 1.92, 0)), 0), 0), 0), vec3(0.4, 0.4, 0.4)), vec3(0.1, 0.1, 0.1))), 1), 0.1),
smooth_union(
scaling_out(
dilated(sdf_cube(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(0.29, 1.92, 0)), 0), 0), 0), vec3(0.4, 0.4, 0.4)), vec3(0.05, -0.49, 0)), 0), 0), 0), vec3(1, 1.05, 0.7))), 0.2), 0.7),
smooth_union(
scaling_out(
dilated(sdf_cube(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(0.29, 1.92, 0)), 0), 0), 0), vec3(0.4, 0.4, 0.4)), vec3(0.06, -0.13, 0)), 0), 0), 0), vec3(0.9, 0.56, 0.91))), 0.2), 0.56),
smooth_union(
scaling_out(
sdf_ellipsoid(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
reflected_z(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(0.29, 1.92, 0)), 0), 0), 0), vec3(0.4, 0.4, 0.4))), vec3(0, -0.21, 1.07)), 0), -0.93), 0), vec3(0.3, 0.3, 0.3)), vec3(1, 1, 0.5)), 0.3),
(sdf_point(scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(0.29, 1.92, 0)), 0), 0), 0), vec3(0.4, 0.4, 0.4))) - 1),
0.2),
0.2),
0.2),
0.2),
0.2), 0.4),
sdf_intersection(
inverted(
position.y),
sdf_ellipsoid(
position, vec3(0.75, 2, 1)))))));
}

float scene_sdf(vec3 position) {
    return roshi_sdf(position);
}

SdfAndMaterial roshi_material(vec3 position) {
    return sdf_union(
SdfAndMaterial(
scaling_out(
scaling_out(
(sdf_point(scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
reflected_z(
scaling_in(
position, vec3(0.1, 0.1, 0.1))), vec3(7.3, 19.17, 1.83)), 0), 0), 0), vec3(0.24, 0.97, 1.24))) - 1), 0.24), 0.1),
vec3(0.0470588, 0.2, 0.0666667)),
sdf_union(
SdfAndMaterial(
scaling_out(
smooth_union(
dilated(sdf_line_segment(
vec3(2.34, 19.72, 4.31),
vec3(7.08, 19.52, 3.26),
reflected_z(
scaling_in(
position, vec3(0.1, 0.1, 0.1)))), 0.2),
smooth_union(
scaling_out(
dilated(sdf_line_segment(
vec3(0, 0, -1),
vec3(0, 0, 1),
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
scaling_in(
position, vec3(0.1, 0.1, 0.1)), vec3(7.3, 19.75, 0)), 0), 0), 0), vec3(0.58, 2.3, 1.57))), 0.1), 0.58),
scaling_out(
(sdf_point(scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
reflected_z(
scaling_in(
position, vec3(0.1, 0.1, 0.1))), vec3(7.18, 19.21, 1.87)), 0), 0), 0), vec3(0.23, 1.23, 1.57))) - 1), 0.23),
0.1),
0.1), 0.1),
vec3(0.490196, 0.12549, 0.0745098)),
sdf_union(
SdfAndMaterial(
scaling_out(
dilated(sdf_union(
difference(scaling_out(
(sdf_point(scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
reflected_z(
scaling_in(
translated(
position, vec3(0, -0.1, 0)), vec3(0.1, 0.1, 0.1))), vec3(5.52, 22.04, 1.29)), 0), 0), 0), vec3(1.19, 1.24, 1.21))) - 1), 1.19), scaling_out(
(sdf_point(scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
reflected_z(
scaling_in(
translated(
position, vec3(0, -0.1, 0)), vec3(0.1, 0.1, 0.1))), vec3(7.46, 20.27, 1.28)), 0), 0), 0), vec3(2.38, 4.93, 1.78))) - 1), 1.78)),
sdf_union(
smooth_union(
scaling_out(
(sdf_point(scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
scaling_in(
translated(
position, vec3(0, -0.1, 0)), vec3(0.1, 0.1, 0.1)), vec3(6.77, 17.21, 0)), 0), 0), 0), vec3(0.71, 1.04, 1.55))) - 1), 0.71),
scaling_out(
cone_sdf(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
reflected_z(
scaling_in(
translated(
position, vec3(0, -0.1, 0)), vec3(0.1, 0.1, 0.1))), vec3(6.81, 13.67, 2.86)), 0.64), 0), 0), vec3(0.75, 3.51, 1.35)),
vec2(1,1)
), 0.75),
0.5),
scaling_out(
cone_sdf(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
scaling_in(
translated(
position, vec3(0, -0.1, 0)), vec3(0.1, 0.1, 0.1)), vec3(6.46, 10.32, -0.14)), 0), 0), 0), vec3(0.83, 5.2, 2)),
vec2(1,1)
), 0.83))), 0.1), 0.1),
vec3(0.870588, 0.780392, 0.780392)),
sdf_union(
SdfAndMaterial(
scaling_out(
smooth_union(
scaling_out(
scaling_out(
(sdf_point(scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
reflected_z(
scaling_in(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(0.29, 1.92, 0)), 0), 0), 0), vec3(0.4, 0.4, 0.4)), vec3(0.1, 0.1, 0.1))), vec3(9.34, -4.26, 0.25)), 0), 0), 0), vec3(1, 1, 1))) - 1), 1), 0.1),
smooth_union(
scaling_out(
dilated(sdf_line_segment(
vec3(10.07, 0.08, 0),
vec3(10.59, -4.16, 0),
scaling_in(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(0.29, 1.92, 0)), 0), 0), 0), vec3(0.4, 0.4, 0.4)), vec3(0.1, 0.1, 0.1))), 1), 0.1),
smooth_union(
scaling_out(
dilated(sdf_cube(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(0.29, 1.92, 0)), 0), 0), 0), vec3(0.4, 0.4, 0.4)), vec3(0.05, -0.49, 0)), 0), 0), 0), vec3(1, 1.05, 0.7))), 0.2), 0.7),
smooth_union(
scaling_out(
dilated(sdf_cube(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(0.29, 1.92, 0)), 0), 0), 0), vec3(0.4, 0.4, 0.4)), vec3(0.06, -0.13, 0)), 0), 0), 0), vec3(0.9, 0.56, 0.91))), 0.2), 0.56),
smooth_union(
scaling_out(
sdf_ellipsoid(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
reflected_z(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(0.29, 1.92, 0)), 0), 0), 0), vec3(0.4, 0.4, 0.4))), vec3(0, -0.21, 1.07)), 0), -0.93), 0), vec3(0.3, 0.3, 0.3)), vec3(1, 1, 0.5)), 0.3),
(sdf_point(scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(0.29, 1.92, 0)), 0), 0), 0), vec3(0.4, 0.4, 0.4))) - 1),
0.2),
0.2),
0.2),
0.2),
0.2), 0.4),
vec3(0.819608, 0.560784, 0.498039)),
SdfAndMaterial(
sdf_intersection(
inverted(
position.y),
sdf_ellipsoid(
position, vec3(0.75, 2, 1))),
vec3(0.8, 0.443137, 0.239216))))));
}

SdfAndMaterial scene_material(vec3 position) {
    return roshi_material(position);
}

