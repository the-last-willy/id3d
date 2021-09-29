// House.

vec3 house_color(in vec3 p) {
    return vec3(255., 153., 230.) / 255.;
}

SDF_Info house(in vec3 p) {
    SDF_Info si;
    si.color = house_color(p);
    si.distance = cube_sdf((p - vec3(0., 7., 0.)) / 5.) * 5.;
    return si;
}

// Island.

vec3 island_color(in vec3 p) {
    return vec3(255., 212., 59.) / 255.;
}

SDF_Info island(in vec3 p) {
    SDF_Info si;
    si.color = island_color(p);
    si.distance = sdf_point(p - vec3(0., -45., 0.)) - 50.;
    return si;
}

// Sea.

vec3 sea_color(in vec3 p) {
    return vec3(0., 0., 1.);
}

SDF_Info sea(in vec3 p) {
    SDF_Info si;
    si.color = sea_color(p);
    si.distance = sdf_plane(p.xzy);
    return si;
}

//

SDF_Info kame_house(in vec3 p) {
    SDF_Info si = sea(p);
    si = sdf_union(si, island(p));
    si = sdf_union(si, house(p));
    return si;
}

vec3 kame_house_normal(in vec3 p) {
    float e = .0001;
    float v = kame_house(p).distance;
    return normalize(vec3(
        kame_house(vec3(p.x + e, p.y, p.z)).distance - v,
        kame_house(vec3(p.x, p.y + e, p.z)).distance - v,
        kame_house(vec3(p.x, p.y, p.z + e)).distance - v));
}
