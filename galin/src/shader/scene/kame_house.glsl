struct SdfAndMaterial {
    vec3 color;
    float distance;
};

float circle(float r, vec3 p) {
    vec2 q = vec2(length(p.xz)-r,p.y);
    return length(q);
}

float cone(in vec2 q, in vec3 p) {
    vec2 w = vec2( length(p.xz), p.y );
    vec2 a = w - q*clamp( dot(w,q)/dot(q,q), 0.0, 1.0 );
    vec2 b = w - q*vec2( clamp( w.x/q.x, 0.0, 1.0 ), 1.0 );
    float k = sign( q.y );
    float d = min(dot( a, a ),dot(b, b));
    float s = max( k*(w.x*q.y-w.y*q.x),k*(w.y-q.y)  );
    return sqrt(d)*sign(s);
}

// vec3 controlled(vec3 p) {
//     return (controls_transform * vec4(p, 1.f)).xyz;
// }

float corrected(float c, float d) {
    return d * c;
}

SdfAndMaterial corrected(float c, SdfAndMaterial sam) {
    return SdfAndMaterial(
        sam.color,
        corrected(c, sam.distance));
}

float cube(vec3 p) {
    p = abs(p);
    float exterior = length(max(p - vec3(.5), 0.));
    float interior = min(max(p.x, max(p.y, p.z)) - .5, 0.);
    return exterior + interior;
}

float difference(float d0, float d1) {
    return max(d0, -d1);
}

SdfAndMaterial difference(in SdfAndMaterial sam0, in SdfAndMaterial sam1) {
    return SdfAndMaterial(
        sam0.color,
        difference(sam0.distance, sam1.distance));
}

float dilated(float radius, float d) {
    return d - radius;
}

SdfAndMaterial dilated(float radius, SdfAndMaterial sam) {
    sam.distance = dilated(radius, sam.distance);
    return sam;
}

float inverted(float d) {
    return -d;
}

SdfAndMaterial inverted(in SdfAndMaterial sam) {
    return SdfAndMaterial(
        sam.color,
        inverted(sam.distance));
}

float onion(in float f) {
    return abs(f);
}

SdfAndMaterial onion(in SdfAndMaterial sam) {
    return SdfAndMaterial(
        sam.color,
        onion(sam.distance));
}

float point(in vec3 position) {
    return length(position);
}

vec3 reflected_x(vec3 p) {
    return vec3(abs(p.x), p.yz);
}

vec3 reflected_y(in vec3 p) {
    p.y = abs(p.y);
    return p;
}

vec3 reflected_z(vec3 p) {
    p.z = abs(p.z);
    return p;
}

vec3 rotated_x(float a, vec3 p) {
    float sa=sin(a);
    float ca=cos(a);
    return vec3(
        p.x,
        ca * p.y - sa * p.z,
        sa * p.y + ca * p.z);
}

vec3 rotated_y(float a, vec3 p) {
    float sa=sin(a);
    float ca=cos(a);
    return vec3(ca*p.x+sa*p.z,p.y,-sa*p.x+ca*p.z);
}

vec3 rotated_z(float a, vec3 p) {
    float sa=sin(a);
    float ca=cos(a);
    return vec3(ca*p.x+sa*p.y,-sa*p.x+ca*p.y,p.z);
}

vec3 scaled(float s, vec3 position) {
    return position / s;
}

vec3 scaled(vec3 s, vec3 position) {
    return position / s;
}

float ellipsoid(in vec3 r, in vec3 p) {
    float k0 = length(p/r);
    float k1 = length(p/(r*r));
    return k0*(k0-1.0)/k1;
}

float intersection(in float d0, in float d1) {
    return max(d0, d1);
}

SdfAndMaterial intersection(in SdfAndMaterial sam0, in SdfAndMaterial sam1) {
    if(sam0.distance < sam1.distance) {
        return sam1;
    } else {
        return sam0;
    }
}

float line_segment(vec3 a, vec3 b, vec3 p) {
    vec3 pa = p - a, ba = b - a;
    float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
    return length( pa - ba*h );
}

float plane(vec3 position) {
    return abs(position.x);
}

float unionn(in float d0, in float d1) {
    return min(d0, d1);
}

SdfAndMaterial unionn(in SdfAndMaterial sam0, in SdfAndMaterial sam1) {
    if(sam0.distance < sam1.distance) {
        return sam0;
    } else {
        return sam1;
    }
}

float smooth_union(float k, float d1, float d2) {
    float h = clamp( 0.5 + 0.5*(d2-d1)/k, 0.0, 1.0 );
    return mix( d2, d1, h ) - k*h*(1.0-h);
}

SdfAndMaterial smooth_union(float k, in SdfAndMaterial sam0, in SdfAndMaterial sam1) {
    float d1 = sam0.distance;
    float d2 = sam1.distance;
    float h = clamp( 0.5 + 0.5*(d2-d1)/k, 0.0, 1.0 );
    return SdfAndMaterial(
        mix(sam1.color, sam0.color, h),
        mix(d2, d1, h) - k*h*(1.0-h));
}

vec3 translated(vec3 translation, vec3 position) {
    return position - translation;
}

#include "kame_house_gen.glsl"

vec3 scene_normal(in vec3 p) {
    float e = .01;
    float v = scene_sdf(p);
    return normalize(vec3(
        scene_sdf(vec3(p.x + e, p.y, p.z)) - v,
        scene_sdf(vec3(p.x, p.y + e, p.z)) - v,
        scene_sdf(vec3(p.x, p.y, p.z + e)) - v));
}
