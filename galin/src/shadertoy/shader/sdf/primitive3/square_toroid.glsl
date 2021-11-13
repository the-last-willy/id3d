float sdf_square_toroid(float r, vec3 v) {
    // XY circle.
    float d = length(v.xy - normalize(v.xy)) - r;;
    // Extrusion.
    vec2 w = vec2(d, abs(v.z) - r);
    return min(max(w.x,w.y),0.) + length(max(w, 0.));
}

float sdf_square_toroid(float r, float h, vec3 v) {
    // XY circle.
    float d = length(v.xy - normalize(v.xy)) - r;;
    // Extrusion.
    vec2 w = vec2(d, abs(v.z) - h);
    return min(max(w.x,w.y),0.) + length(max(w, 0.));
}
