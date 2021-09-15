float cylinder_sdf(vec3 p) {
    vec2 q = vec2(length(p.xy), p.z);
    return square_sdf(q);
}
