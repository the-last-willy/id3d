float roshi_rod_sdf(vec3 position) {
    return smooth_union(
dilated(scaling_out(
scaling_out(
sdf_cube(
scaling_in(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(0.33, 1.01, -0.5)), -1.01), -0.21), -0.73), vec3(0.23, 0.24, 0.58)), vec3(0.1, 0.1, 0.1))), 0.1), 0.23), 0.05),
smooth_union(
dilated(scaling_out(
scaling_out(
sdf_cube(
scaling_in(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(0.33, 1.19, -0.5)), -0.92), -0.46), 0), vec3(1.76, 1.59, 1.66)), vec3(0.1, 0.1, 0.1))), 0.1), 1.59), 0.05),
smooth_union(
dilated(scaling_out(
scaling_out(
sdf_cube(
scaling_in(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(0.33, 1.43, -0.5)), -1.01), -0.21), -0.73), vec3(1.11, 1, 1.55)), vec3(0.1, 0.1, 0.1))), 0.1), 1), 0.05),
scaling_out(
dilated(sdf_line_segment(
vec3(0, -1, 0),
vec3(0, 1, 0),
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(0.36, -0.02, -0.46)), 0), 0), 0), vec3(1, 1.08, 1))), 0.05), 1),
0.2),
0.2),
0.2);
}

float roshi_lower_half_sdf(vec3 position) {
    return smooth_union(
scaling_out(
(sdf_point(scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(-0.03, -0.26, -0.31)), 0), 0), 0), vec3(0.331, 0.32, 0.36))) - 1), 0.32),
smooth_union(
scaling_out(
(sdf_point(scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(0.1, -0.11, 0.36)), 0), 0), 0), vec3(0.31, 0.32, 0.36))) - 1), 0.31),
sdf_union(
scaling_out(
sdf_intersection(
inverted(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
reflected_z(
position), vec3(0.17, -1.25, 0.3)), 0), 0.37), 0), vec3(0.51, 0.26, 0.24)).y),
sdf_union(
scaling_out(
(sdf_point(scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
reflected_z(
position), vec3(0.17, -1.25, 0.3)), 0), 0.37), 0), vec3(0.51, 0.26, 0.24)), vec3(-0.08, 0.12, 0)), 0), 0), 0), vec3(1.29, 0.74, 1.21))) - 1), 0.74),
(sdf_point(scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
reflected_z(
position), vec3(0.17, -1.25, 0.3)), 0), 0.37), 0), vec3(0.51, 0.26, 0.24))) - 1))), 0.24),
scaling_out(
dilated(cone_sdf(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
reflected_z(
position), vec3(0.01, -0.88, 0.24)), 0), 0), 0), vec3(0.49, 0.94, 0.53)),
vec2(1,1)
), 0.2), 0.49)),
0.2),
0.2);
}

float roshi_sdf(vec3 position) {
    return sdf_union(
roshi_lower_half_sdf(position),
sdf_union(
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
0.3),
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
sdf_union(
dilated(scaling_out(
sdf_cube(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(0, 0.2, 0.92)), 0), 0), 0), vec3(0.125, 0.13, 0.15))), 0.125), 0.1),
sdf_union(
scaling_out(
dilated(circle(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(0, 0.33, 0.86)), 0), 0), 0), vec3(0.38, 0.96, 0.36)),
1), 0.2), 0.36),
sdf_union(
smooth_union(
dilated(scaling_out(
sdf_cube(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(0.73, 0.61, -0.9)), 0), 0), 0), vec3(0.125, 0.13, 0.15))), 0.125), 0.1),
dilated(sdf_line_segment(
vec3(0, 0.65, -0.98),
vec3(0.67, 0.63, -0.94),
position), 0.1),
0.2),
sdf_union(
scaling_out(
dilated(circle(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(0.36, 0.62, -0.94)), 0), 0), -1.68), vec3(0.36, 1.26, 0.36)),
1), 0.2), 0.36),
sdf_union(
scaling_out(
sdf_union(
(sdf_point(controlled(scaling_in(
position, vec3(0.1, 0.1, 0.1)))) - 1),
sdf_union(
scaling_out(
(sdf_point(scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
scaling_in(
position, vec3(0.1, 0.1, 0.1)), vec3(6.15, 4.54, 0)), 0), 0), 0), vec3(0.62, 0.39, 1))) - 1), 0.39),
sdf_union(
scaling_out(
(sdf_point(scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
scaling_in(
position, vec3(0.1, 0.1, 0.1)), vec3(5.95, 7.06, 0)), 0), 0), 0), vec3(0.62, 0.39, 1))) - 1), 0.39),
scaling_out(
(sdf_point(scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
scaling_in(
position, vec3(0.1, 0.1, 0.1)), vec3(5.73, 9.51, 0)), 0), 0), 0), vec3(0.62, 0.39, 1))) - 1), 0.39)))), 0.1),
sdf_union(
sdf_intersection(
inverted(
translated(
position, vec3(0.1, 0, 0)).x),
scaling_out(
sdf_intersection(
inverted(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(-0.06, 0, 0)), 0), 0), 0), vec3(1.14, 1.16, 0.46)).y),
sdf_ellipsoid(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(-0.06, 0, 0)), 0), 0), 0), vec3(1.14, 1.16, 0.46)), vec3(0.65, 2, 1))), 0.46)),
sdf_union(
sdf_union(
sdf_intersection(
inverted(
translated(
position, vec3(0, 0.2, 0)).y),
dilated(sdf_line_segment(
vec3(0, 1.16, 0.58),
vec3(0, 0.27, 0.87),
position), 0.4)),
sdf_union(
sdf_intersection(
scaling_out(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(0.47, 0, 0)), 0), 0), 0), vec3(1, 1, 1)).x, 1),
dilated(onion(sdf_union(
dilated(sdf_line_segment(
vec3(0, 0.65, -0.98),
vec3(0.37, 0.63, -0.94),
position), 0.39),
dilated(sdf_line_segment(
vec3(0, 1.16, -0.58),
vec3(0, 0.65, -0.98),
position), 0.39))), 0.01)),
smooth_union(
scaling_out(
(sdf_point(scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
reflected_z(
position), vec3(0, 1.16, 0.58)), 0), 0), 0), vec3(0.4, 0.4, 0.4))) - 1), 0.4),
sdf_intersection(
inverted(
position.y),
sdf_ellipsoid(
position, vec3(0.65, 2, 1))),
0.1))),
scaling_out(
roshi_rod_sdf(scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(0.36, -0.02, -0.46)), 0), 0), 0), vec3(1, 1.08, 1))), 1)))))))))))));
}

float scene_sdf(vec3 position) {
    return roshi_sdf(position);
}

SdfAndMaterial roshi_rod_material(vec3 position) {
    return SdfAndMaterial(
smooth_union(
dilated(scaling_out(
scaling_out(
sdf_cube(
scaling_in(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(0.33, 1.01, -0.5)), -1.01), -0.21), -0.73), vec3(0.23, 0.24, 0.58)), vec3(0.1, 0.1, 0.1))), 0.1), 0.23), 0.05),
smooth_union(
dilated(scaling_out(
scaling_out(
sdf_cube(
scaling_in(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(0.33, 1.19, -0.5)), -0.92), -0.46), 0), vec3(1.76, 1.59, 1.66)), vec3(0.1, 0.1, 0.1))), 0.1), 1.59), 0.05),
smooth_union(
dilated(scaling_out(
scaling_out(
sdf_cube(
scaling_in(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(0.33, 1.43, -0.5)), -1.01), -0.21), -0.73), vec3(1.11, 1, 1.55)), vec3(0.1, 0.1, 0.1))), 0.1), 1), 0.05),
scaling_out(
dilated(sdf_line_segment(
vec3(0, -1, 0),
vec3(0, 1, 0),
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(0.36, -0.02, -0.46)), 0), 0), 0), vec3(1, 1.08, 1))), 0.05), 1),
0.2),
0.2),
0.2),
vec3(0.356863, 0.160784, 0.152941));
}

SdfAndMaterial roshi_lower_half_material(vec3 position) {
    return smooth_union(
SdfAndMaterial(
scaling_out(
(sdf_point(scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(-0.03, -0.26, -0.31)), 0), 0), 0), vec3(0.331, 0.32, 0.36))) - 1), 0.32),
vec3(0.219608, 0.203922, 0.494118)),
smooth_union(
SdfAndMaterial(
scaling_out(
(sdf_point(scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(0.1, -0.11, 0.36)), 0), 0), 0), vec3(0.31, 0.32, 0.36))) - 1), 0.31),
vec3(0.219608, 0.203922, 0.494118)),
sdf_union(
scaling_out(
sdf_intersection(
SdfAndMaterial(
inverted(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
reflected_z(
position), vec3(0.17, -1.25, 0.3)), 0), 0.37), 0), vec3(0.51, 0.26, 0.24)).y),
vec3(0.819608, 0.560784, 0.498039)),
sdf_union(
SdfAndMaterial(
scaling_out(
(sdf_point(scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
reflected_z(
position), vec3(0.17, -1.25, 0.3)), 0), 0.37), 0), vec3(0.51, 0.26, 0.24)), vec3(-0.08, 0.12, 0)), 0), 0), 0), vec3(1.29, 0.74, 1.21))) - 1), 0.74),
vec3(0.227451, 0.145098, 0.262745)),
SdfAndMaterial(
(sdf_point(scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
reflected_z(
position), vec3(0.17, -1.25, 0.3)), 0), 0.37), 0), vec3(0.51, 0.26, 0.24))) - 1),
vec3(0.819608, 0.560784, 0.498039)))), 0.24),
SdfAndMaterial(
scaling_out(
dilated(cone_sdf(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
reflected_z(
position), vec3(0.01, -0.88, 0.24)), 0), 0), 0), vec3(0.49, 0.94, 0.53)),
vec2(1,1)
), 0.2), 0.49),
vec3(0.219608, 0.203922, 0.494118))),
0.2),
0.2);
}

SdfAndMaterial roshi_material(vec3 position) {
    return sdf_union(
roshi_lower_half_material(position),
sdf_union(
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
0.3),
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
sdf_union(
SdfAndMaterial(
dilated(scaling_out(
sdf_cube(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(0, 0.2, 0.92)), 0), 0), 0), vec3(0.125, 0.13, 0.15))), 0.125), 0.1),
vec3(0.819608, 0.560784, 0.498039)),
sdf_union(
SdfAndMaterial(
scaling_out(
dilated(circle(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(0, 0.33, 0.86)), 0), 0), 0), vec3(0.38, 0.96, 0.36)),
1), 0.2), 0.36),
vec3(0.870588, 0.780392, 0.780392)),
sdf_union(
SdfAndMaterial(
smooth_union(
dilated(scaling_out(
sdf_cube(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(0.73, 0.61, -0.9)), 0), 0), 0), vec3(0.125, 0.13, 0.15))), 0.125), 0.1),
dilated(sdf_line_segment(
vec3(0, 0.65, -0.98),
vec3(0.67, 0.63, -0.94),
position), 0.1),
0.2),
vec3(0.819608, 0.560784, 0.498039)),
sdf_union(
SdfAndMaterial(
scaling_out(
dilated(circle(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(0.36, 0.62, -0.94)), 0), 0), -1.68), vec3(0.36, 1.26, 0.36)),
1), 0.2), 0.36),
vec3(0.870588, 0.780392, 0.780392)),
sdf_union(
SdfAndMaterial(
scaling_out(
sdf_union(
(sdf_point(controlled(scaling_in(
position, vec3(0.1, 0.1, 0.1)))) - 1),
sdf_union(
scaling_out(
(sdf_point(scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
scaling_in(
position, vec3(0.1, 0.1, 0.1)), vec3(6.15, 4.54, 0)), 0), 0), 0), vec3(0.62, 0.39, 1))) - 1), 0.39),
sdf_union(
scaling_out(
(sdf_point(scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
scaling_in(
position, vec3(0.1, 0.1, 0.1)), vec3(5.95, 7.06, 0)), 0), 0), 0), vec3(0.62, 0.39, 1))) - 1), 0.39),
scaling_out(
(sdf_point(scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
scaling_in(
position, vec3(0.1, 0.1, 0.1)), vec3(5.73, 9.51, 0)), 0), 0), 0), vec3(0.62, 0.39, 1))) - 1), 0.39)))), 0.1),
vec3(0.227451, 0.145098, 0.262745)),
sdf_union(
SdfAndMaterial(
sdf_intersection(
inverted(
translated(
position, vec3(0.1, 0, 0)).x),
scaling_out(
sdf_intersection(
inverted(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(-0.06, 0, 0)), 0), 0), 0), vec3(1.14, 1.16, 0.46)).y),
sdf_ellipsoid(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(-0.06, 0, 0)), 0), 0), 0), vec3(1.14, 1.16, 0.46)), vec3(0.65, 2, 1))), 0.46)),
vec3(0.870588, 0.780392, 0.780392)),
sdf_union(
SdfAndMaterial(
sdf_union(
sdf_intersection(
inverted(
translated(
position, vec3(0, 0.2, 0)).y),
dilated(sdf_line_segment(
vec3(0, 1.16, 0.58),
vec3(0, 0.27, 0.87),
position), 0.4)),
sdf_union(
sdf_intersection(
scaling_out(
scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(0.47, 0, 0)), 0), 0), 0), vec3(1, 1, 1)).x, 1),
dilated(onion(sdf_union(
dilated(sdf_line_segment(
vec3(0, 0.65, -0.98),
vec3(0.37, 0.63, -0.94),
position), 0.39),
dilated(sdf_line_segment(
vec3(0, 1.16, -0.58),
vec3(0, 0.65, -0.98),
position), 0.39))), 0.01)),
smooth_union(
scaling_out(
(sdf_point(scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
reflected_z(
position), vec3(0, 1.16, 0.58)), 0), 0), 0), vec3(0.4, 0.4, 0.4))) - 1), 0.4),
sdf_intersection(
inverted(
position.y),
sdf_ellipsoid(
position, vec3(0.65, 2, 1))),
0.1))),
vec3(0.8, 0.443137, 0.239216)),
scaling_out(
roshi_rod_material(scaling_in(
rotated_z(
rotated_y(
rotated_x(
translated(
position, vec3(0.36, -0.02, -0.46)), 0), 0), 0), vec3(1, 1.08, 1))), 1)))))))))))));
}

SdfAndMaterial scene_material(vec3 position) {
    return roshi_material(position);
}

