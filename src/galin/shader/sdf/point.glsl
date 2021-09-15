float point_sdf(vec2 v) {
    return length(v);
}

float point_sdf(vec3 v) {
    return length(v);
}

float point_sdf(vec3 p, vec3 q) {
    return length(p - q);
}
