vec3 kame_house_color(in vec3 p) {
    return vec3(1., 0., 0.);
}

float kame_house_distance(in vec3 p) {
    return sdf_sphere(p);
}
