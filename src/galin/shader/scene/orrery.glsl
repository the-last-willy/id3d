
// Props.

float base(in vec3 p) {
    p -= vec3(0., -4., 0.);
    float d = sdf_intersection(point_sdf(p) - 25.f, plane_sdf(p.xzy));
    float s = 3.;
    d = sdf_difference(d, sdf_square_toroid(0.2 / s, 0.1 / s, p.xzy / s) * s);
    s = 6.;
    d = sdf_difference(d, sdf_square_toroid(0.2 / s, 0.1 / s, p.xzy / s) * s);
    s = 9.;
    d = sdf_difference(d, sdf_square_toroid(0.2 / s, 0.1 / s, p.xzy / s) * s);
    s = 12.;
    d = sdf_difference(d, sdf_square_toroid(0.2 / s, 0.1 / s, p.xzy / s) * s);
    s = 15.;
    d = sdf_difference(d, sdf_square_toroid(0.2 / s, 0.1 / s, p.xzy / s) * s);
    s = 18.;
    d = sdf_difference(d, sdf_square_toroid(0.2 / s, 0.1 / s, p.xzy / s) * s);
    s = 21.;
    d = sdf_difference(d, sdf_square_toroid(0.2 / s, 0.1 / s, p.xzy / s) * s);
    s = 24.;
    d = sdf_difference(d, sdf_square_toroid(0.2 / s, 0.1 / s, p.xzy / s) * s);
    return d;
}

// float pillar(in vec3 p) {
//     return float(
//         vec3(252, 212, 64) / 255,
//         point_sdf(p, vec3(0., 0., 0.), 1., 1.));
// }

// Planets.

vec3 sun_position() {
    return vec3(0., 0., 0.);
}

float sun_disc(in vec3 p) {
    float d = point_sdf(p) - 1.;
    d = sdf_union(d, segment_sdf(vec3(0., -5., 0.), vec3(0., 0., 0.), p) - .2);
    return d;
}

float mercury_disc(in vec3 p) {
    p = rotateY(p, -1.7 * iTime);
    p -= vec3(3., 0., 0.);

    float d = point_sdf(p) - 1.;
    d = sdf_union(d, segment_sdf(vec3(0., -5., 0.), vec3(0., 0., 0.), p) - .2);
    return d;
}

float venus_disc(in vec3 p) {
    p = rotateY(p, -1.6 * iTime);
    p -= vec3(6., 0., 0.);

    float d = point_sdf(p) - 1.;
    d = sdf_union(d, segment_sdf(vec3(0., -5., 0.), vec3(0., 0., 0.), p) - .2);
    return d;
}

float moon_distance(in vec3 p) {
    p -= rotateY(vec3(1.5, 0., 0.), 0);
    float d = point_sdf(p) - 0.25;
    d = sdf_union(d, segment_sdf(vec3(0., -2., 0.), vec3(0., 0., 0.), p) - .05);
    d = sdf_union(d, segment_sdf(vec3(0., -2., 0.), vec3(-1.5, -2., 0.), p) - .05);
    return d;
}

float earth_disc(in vec3 p) {
    p = rotateY(p, -1.5 * iTime);
    p -= vec3(9., 0., 0.);
    p = rotateY(p, -10.f * iTime);

    float d = point_sdf(p) - 1.;
    d = sdf_union(d, segment_sdf(vec3(0., -5., 0.), vec3(0., 0., 0.), p) - .2);
    
    d = sdf_union(d, moon_distance(p));
    
    return d;
}

float mars_disc(in vec3 p) {
    p = rotateY(p, -1.4 * iTime);
    p -= vec3(12., 0., 0.);

    float d = point_sdf(p) - 1.;
    d = sdf_union(d, segment_sdf(vec3(0., -5., 0.), vec3(0., 0., 0.), p) - .2);
    return d;
}

float jupiter_disc(in vec3 p) {
    p = rotateY(p, -1.3 * iTime);
    p -= vec3(15., 0., 0.);

    float d = point_sdf(p) - 1.;
    d = sdf_union(d, segment_sdf(vec3(0., -5., 0.), vec3(0., 0., 0.), p) - .2);
    return d;
}

float saturn_disc(in vec3 p) {
    p = rotateY(p, -1.2 * iTime);
    p -= vec3(18., 0., 0.);
    p = rotateY(p, -1.2 * iTime);

    float d = point_sdf(p) - 1.;
    d = sdf_union(d, segment_sdf(vec3(0., -5., 0.), vec3(0., 0., 0.), p) - .2);

    vec3 q = p / 1.5;
    q = rotateX(q, 1.);
    d = sdf_union(d, sdf_square_toroid(0.2, 0.02, q) / 1.5);
    d = sdf_union(d, segment_sdf(vec3(0.), vec3(0., -1., 0.), q) - 0.05);
    return d;
}

float neptune_disc(in vec3 p) {
    p = rotateY(p, -1.1 * iTime);
    p -= vec3(21., 0., 0.);

    float d = point_sdf(p) - 1.;
    d = sdf_union(d, segment_sdf(vec3(0., -5., 0.), vec3(0., 0., 0.), p) - .2);
    return d;
}

float uranus_disc(in vec3 p) {
    p = rotateY(p, -1 * iTime);
    p -= vec3(24., 0., 0.);

    float d = point_sdf(p) - 1.;
    d = sdf_union(d, segment_sdf(vec3(0., -5., 0.), vec3(0., 0., 0.), p) - .2);
    return d;
}

float orrery_distance(in vec3 p) {
    float d = 1000.;
    d = sdf_union(d, sun_disc(p));
    d = sdf_union(d, mercury_disc(p));
    d = sdf_union(d, venus_disc(p));
    d = sdf_union(d, earth_disc(p));
    d = sdf_union(d, mars_disc(p));
    d = sdf_union(d, jupiter_disc(p));
    d = sdf_union(d, saturn_disc(p));
    d = sdf_union(d, neptune_disc(p));
    d = sdf_union(d, uranus_disc(p));

    d = sdf_union(d, base(p));
    return d;
}

vec3 orrery_normal(in vec3 p) {
    float e = .0001;
    vec3 n;
    float v = orrery_distance(p);
    n.x = orrery_distance(vec3(p.x + e, p.y, p.z)) - v;
    n.y = orrery_distance(vec3(p.x, p.y + e, p.z)) - v;
    n.z = orrery_distance(vec3(p.x, p.y, p.z + e)) - v;
    return normalize(n);
}
