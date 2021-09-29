float square_sdf(vec2 p) {
    p = abs(p);
    float exterior = length(max(p - vec2(.5), 0.));
    float interior = min(max(p.x, p.y) - .5, 0.);
    return exterior + interior;
}

float cube_sdf(vec3 p) {
    p = abs(p);
    float exterior = length(max(p - vec3(.5), 0.));
    float interior = min(max(p.x, max(p.y, p.z)) - .5, 0.);
    return exterior + interior;
}
