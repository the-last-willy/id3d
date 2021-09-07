float circle_sdf(vec3 v) {
    return length(v - vec3(normalize(v.xy), 0.));
}
