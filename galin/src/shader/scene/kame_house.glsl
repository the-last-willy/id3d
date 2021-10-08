struct SdfAndMaterial {
    float distance;

    vec3 color;
};

vec3 attracted(vec3 p, vec3 attractor, float intensity) {
    return p - normalize(p - attractor) * intensity;
}

vec3 attracted(vec3 p, float intensity) {
    return p - normalize(p) * intensity;
}

vec3 controlled(vec3 p) {
    return (controls_transform * vec4(p, 1.f)).xyz;
}

float dilated(float d, float radius) {
    return d - radius;
}

SdfAndMaterial dilated(SdfAndMaterial sam, float radius) {
    sam.distance = dilated(sam.distance, radius);
    return sam;
}

float inverted(float d) {
    return -d;
}

SdfAndMaterial inverted(in SdfAndMaterial sam) {
    return SdfAndMaterial(
        inverted(sam.distance),
        sam.color);
}

SdfAndMaterial mul(in SdfAndMaterial sam, float f) {
    return SdfAndMaterial(
        sam.distance * f,
        sam.color);
}

float onion(in float f) {
    return abs(f);
}

SdfAndMaterial onion(in SdfAndMaterial sam) {
    return SdfAndMaterial(
        onion(sam.distance),
        sam.color);
}

vec3 reflected_x(vec3 p) {
    return vec3(abs(p.x), p.yz);
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

SdfAndMaterial sdf_intersection(in SdfAndMaterial sam0, in SdfAndMaterial sam1) {
    if(sam0.distance < sam1.distance) {
        return sam1;
    } else {
        return sam0;
    }
}

float sdf_line_segment(vec3 a, vec3 b, vec3 p) {
    vec3 pa = p - a, ba = b - a;
    float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
    return length( pa - ba*h );
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









#include "kame_house_gen.glsl"














vec3 scene_normal(in vec3 p) {
    float e = .01;
    float v = scene_sdf(p);
    return normalize(vec3(
        scene_sdf(vec3(p.x + e, p.y, p.z)) - v,
        scene_sdf(vec3(p.x, p.y + e, p.z)) - v,
        scene_sdf(vec3(p.x, p.y, p.z + e)) - v));
}
