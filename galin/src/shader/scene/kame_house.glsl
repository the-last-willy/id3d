// House.

// vec3 house_color(in vec3 p) {
//     return vec3(255., 153., 230.) / 255.;
// }

// SDF_Info house(in vec3 p) {
//     SDF_Info si;
//     si.color = house_color(p);
//     si.distance = cube_sdf((p - vec3(0., 7., 0.)) / 5.) * 5.;
//     return si;
// }

// // Island.

// vec3 island_color(in vec3 p) {
//     return vec3(255., 212., 59.) / 255.;
// }

// SDF_Info island(in vec3 p) {
//     SDF_Info si;
//     si.color = island_color(p);
//     si.distance = sdf_point(p - vec3(0., -45., 0.)) - 50.;
//     return si;
// }

// Sea.

// vec3 sea_color(in vec3 p) {
//     return vec3(0., 0., 1.);
// }

// SDF_Info sea(in vec3 p) {
//     SDF_Info si;
//     si.color = sea_color(p);
//     si.distance = sdf_plane(p.xzy);
//     return si;
// }

// //

// SDF_Info kame_house(in vec3 p) {
//     SDF_Info si = sea(p);
//     si = sdf_union(si, island(p));
//     si = sdf_union(si, house(p));
//     return si;
// }

// vec3 kame_house_normal(in vec3 p) {
//     float e = .0001;
//     float v = kame_house(p).distance;
//     return normalize(vec3(
//         kame_house(vec3(p.x + e, p.y, p.z)).distance - v,
//         kame_house(vec3(p.x, p.y + e, p.z)).distance - v,
//         kame_house(vec3(p.x, p.y, p.z + e)).distance - v));
// }

struct SdfAndMaterial {
    float distance;

    vec3 color;
};

SdfAndMaterial mul(in SdfAndMaterial sam, float f) {
    return SdfAndMaterial(
        sam.distance * f,
        sam.color);
}

vec3 rotated_x(vec3 p, float a) {
  float sa=sin(a);
  float ca=cos(a);
  return vec3(
      p.x,
      ca * p.y - sa * p.z,
      sa * p.y + ca * p.z);
}

vec3 rotated_y(vec3 p, float a) {
  float sa=sin(a);
  float ca=cos(a);
  return vec3(ca*p.x+sa*p.z,p.y,-sa*p.x+ca*p.z);
}

vec3 rotated_z(vec3 p, float a) {
  float sa=sin(a);
  float ca=cos(a);
  return vec3(ca*p.x+sa*p.y,-sa*p.x+ca*p.y,p.z);
}

vec3 scaling_in(vec3 position, float scaling) {
    return position / scaling;
}

vec3 scaling_in(vec3 position, vec3 scaling) {
    return position / scaling;
}

float scaling_out(float f, float scaling) {
    return f * scaling;
}

SdfAndMaterial scaling_out(SdfAndMaterial sam, float scaling) {
    SdfAndMaterial so = sam;
    so.distance *= scaling;
    return so; 
}

float sdf_ellipsoid(in vec3 p, in vec3 r) {
    float k0 = length(p/r);
    float k1 = length(p/(r*r));
    return k0*(k0-1.0)/k1;
}

float sdf_plane(vec3 position) {
    return abs(position.x);
}

SdfAndMaterial sdf_union(in SdfAndMaterial sam0, in SdfAndMaterial sam1) {
    if(sam0.distance < sam1.distance) {
        return sam0;
    } else {
        return sam1;
    }
}

vec3 translated(vec3 position, vec3 translation) {
    return position - translation;
}







// ColoredDistance closest_color(ColoredDistance cd0, ColoredDistance cd1) {
//     return (cd0.distance < cd1.distance) ? cd0 : cd1;
// }

// float house_distance(vec3 position) {
//         return (sdf_cube(((position - vec3(0, 7, 0)) / 5)) * 5);
// }

// float island_distance(vec3 position) {
//         return (length((position - vec3(0, -45, 0))) - 50);
// }

// float sea_distance(vec3 position) {
//         return sdf_plane(position.xzy);
// }











float sea_sdf(vec3 position) {
    return (sdf_plane(position.yxz) - 0.5);
}

float palm_tree_sdf(vec3 position) {
    return sdf_union(
(line_sdf(
vec3(2, 0, 2),
vec3(5, 5, 5),
position) - 1.),
(sdf_point(translated(position, vec3(5, 5, 5))) - 1));
}

float island_sdf(vec3 position) {
    return scaling_out((sdf_point(scaling_in(translated(position, vec3(0, -8, 0)), 10)) - 1), 10);     
}

float house_ground_floor_sdf(vec3 position) {
    return sdf_cube(position);
}

float house_roof_sdf(vec3 position) {
    return scaling_out(sdf_cube(rotated_z(scaling_in(translated(position, vec3(0, 0.5, 0)), vec3(0.707107, 1, 1)), 0.785398)), 1);
}

float house_sdf(vec3 position) {
    return sdf_union(
house_ground_floor_sdf(position),
house_roof_sdf(position));
}

float scene_sdf(vec3 position) {
    return sdf_union(
sdf_union(
sea_sdf(position),
palm_tree_sdf(position)),
sdf_union(
island_sdf(position),
house_sdf(translated(position, vec3(0, 2, 0)))));
}

SdfAndMaterial sea_material(vec3 position) {
    return SdfAndMaterial(
(sdf_plane(position.yxz) - 0.5),
vec3(0, 0, 1));
}

SdfAndMaterial palm_tree_material(vec3 position) {
    return sdf_union(
SdfAndMaterial(
(line_sdf(
vec3(2, 0, 2),
vec3(5, 5, 5),
position) - 0.5),
vec3(0.647059, 0.164706, 0.164706)),
SdfAndMaterial(
(sdf_point(translated(position, vec3(5, 5, 5))) - 1),
vec3(0, 1, 0)));
}

SdfAndMaterial island_material(vec3 position) {
    return SdfAndMaterial(
scaling_out((sdf_point(scaling_in(translated(position, vec3(0, -8, 0)), 10)) - 1), 10),
vec3(1, 1, 0));
}

SdfAndMaterial house_ground_floor_material(vec3 position) {
    return SdfAndMaterial(
sdf_cube(position),
vec3(1, 0, 0));
}

SdfAndMaterial house_roof_material(vec3 position) {
    return SdfAndMaterial(
scaling_out(sdf_cube(rotated_z(scaling_in(translated(position, vec3(0, 0.5, 0)), vec3(0.707107, 1, 1)), 0.785398)), 1),
vec3(1, 0, 0));
}

SdfAndMaterial house_material(vec3 position) {
    return sdf_union(
house_ground_floor_material(position),
house_roof_material(position));
}

SdfAndMaterial scene_material(vec3 position) {
    return sdf_union(
sdf_union(
sea_material(position),
palm_tree_material(position)),
sdf_union(
island_material(position),
house_material(translated(position, vec3(0, 2, 0)))));
}














vec3 scene_normal(in vec3 p) {
    float e = .01;
    float v = scene_sdf(p);
    return normalize(vec3(
        scene_sdf(vec3(p.x + e, p.y, p.z)) - v,
        scene_sdf(vec3(p.x, p.y + e, p.z)) - v,
        scene_sdf(vec3(p.x, p.y, p.z + e)) - v));
}
