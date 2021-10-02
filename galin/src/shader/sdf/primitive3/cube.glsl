float sdf_cube(in vec3 position) {
    vec3 ap = abs(position);
    float exterior = length(max(ap - vec3(.5), 0.));
    float interior = min(max(ap.x, max(ap.y, ap.z)) - .5, 0.);
    return exterior + interior;
}
