float circle_sdf(vec2 v) {
    return length(v - normalize(v));
}

// XY circle.
float circle_sdf(vec3 v) {
    return length(v - vec3(normalize(v.xy), 0.));
}
