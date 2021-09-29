#include "shadertoy_prefix.fs"

struct SDF_Info {
    vec3 color;
    float distance;
};

#include "sdf/all.glsl"

uniform mat4 view_transform;

// subroutine vec3 RenderMode();

// subroutine(RenderMode) vec3 render_albedo() {
//     return vec3(0.);
// }

// subroutine uniform RenderMode render_mode;

// Common

// Hashing function
// Returns a random number in [-1,1]
float Hash(float seed) {
    return fract(sin(seed) * 43758.5453);
}

// Cosine direction
vec3 Cosine(in float seed, in vec3 nor) {
    float u = Hash(78.233 + seed);
    float v = Hash(10.873 + seed);

    // method 3 by fizzer: http://www.amietia.com/lambertnotangent.html
    float a = 6.2831853 * v;
    u = 2. * u - 1.;
    return normalize(nor + vec3(sqrt(1. - u * u) * vec2(cos(a), sin(a)), u));
}

// Rotation matrix around z axis
// a : Angle
mat3 rotate_z(float a) {
    float sa = sin(a);
    float ca = cos(a);
    return mat3(
         ca, sa, 0.,
        -sa, ca, 0., 
         0., 0., 1.);
}

// Compute the ray
// m : Mouse position
// p : Pixel
// ro, rd : Ray origin and direction
void Ray(in vec2 m, in vec2 p, out vec3 ro, out vec3 rd) {
    float a = 3. * 3.14 * m.x;
    float le = 3.8;

    ro = vec3(20., 0., 5.);
    ro *= rotate_z(3. * 3.14 * m.x);

    vec3 ta = vec3(0., 0., 1.);
    vec3 ww = normalize(ta - ro);
    vec3 uu = normalize(cross(ww, vec3(0., 0., 1.)));
    vec3 vv = normalize(cross(uu, ww));
    rd = normalize(p.x * uu + p.y * vv + le * ww);
}

// Main

uniform int Steps = 1000;
const float Epsilon = .03; // Marching epsilon

uniform float T = .5;;

const float rA = 0.1; // Maximum and minimum ray marching or sphere tracing distance from origin
const float rB = 100.;

// Transforms
vec3 rotateX(vec3 p, float a) {
    float sa = sin(a);
    float ca = cos(a);
    return vec3(p.x, ca * p.y - sa * p.z, sa * p.y + ca * p.z);
}

vec3 rotateY(vec3 p, float a) {
    float sa = sin(a);
    float ca = cos(a);
    return vec3(ca * p.x + sa * p.z, p.y, -sa * p.x + ca * p.z);
}

vec3 rotateZ(vec3 p, float a) {
    float sa = sin(a);
    float ca = cos(a);
    return vec3(ca * p.x + sa * p.y, -sa * p.x + ca * p.y, p.z);
}

#include "scene/kame_house.glsl"
#include "scene/orrery.glsl"

// Trace ray using ray marching
// o : ray origin
// u : ray direction
// h : hit
// s : Number of steps
// float Trace(vec3 o, vec3 u, float rB, out bool h, out int s) {
//     h = false;

//     // Don't start at the origin, instead move a little bit forward
//     float t = rA;

//     for(int i = 0; i < Steps; i++) {
//         s = i;
//         vec3 p = o + t * u;
//         float v = Object(p);
//         // Hit object
//         if(v > 0.) {
//             s = i;
//             h = true;
//             break;
//         }
//         // Move along ray
//         t += Epsilon;
//         // Escape marched far away
//         if(t > rB) {
//             break;
//         }
//     }
//     return t;
// }

// Trace ray using ray marching
// o : ray origin
// u : ray direction
// h : hit
// s : Number of steps
float SphereTrace(vec3 o, vec3 u, float rB, out bool h, out int s) {
    h = false;

    // Don't start at the origin, instead move a little bit forward
    float t = rA;

    for (int i = 0; i < Steps; i++)
    {
        s = i;
        vec3 p = o + t * u;
        float v = kame_house(p).distance;
        // Hit object
        if (v < 0.)
        {
            s = i;
            h = true;
            break;
        }
        // Move along ray
        t += max(Epsilon, v);
        // Escape marched far away
        if (t > rB)
        {
            break;
        }
    }
    return t;
}

// Background color
vec3 background(vec3 rd) {
    return mix(vec3(.652, .451, .995), vec3(.552, .897, .995), rd.z * .5 + .5);
}

// Shading with number of steps
vec3 ShadeSteps(int n) {
    float t = float(n) / (float(Steps - 1));
    return .5 + mix(vec3(.05, .05, .5), vec3(.65, .39, .65), t);
}

// Picture in picture
// pixel : Pixel
// pip : Boolean, true if pixel was in sub-picture zone
vec2 Pip(in vec2 pixel, out bool pip) {
    // Pixel coordinates
    vec2 p = (-iResolution.xy + 2. * pixel) / iResolution.y;
    if (pip == true)
    {
        const float fraction = 1. / 4.;
        // Recompute pixel coordinates in sub-picture
        if ((pixel.x < iResolution.x * fraction) && (pixel.y < iResolution.y * fraction))
        {
            p = (-iResolution.xy * fraction + 2. * pixel) / (iResolution.y * fraction);
            pip = true;
        }
        else
        {
            pip = false;
        }
    }
    return p;
}

// Image
void mainImage(out vec4 color, in vec2 pxy) {
    vec2 coords = (2. * pxy - iResolution.xy) / iResolution.y;
    color = vec4(length(coords));

    vec3 rd = normalize(vec3(coords, 1.));
    rd = (view_transform * vec4(rd, 0.f)).xyz;
    vec3 ro = view_transform[3].xyz;

    // Trace ray

    // Hit and number of steps
    bool hit;
    int s;

    float t = SphereTrace(ro, rd, 100., hit, s);

    // Position
    vec3 pos = ro + t * rd;

    // Shade background
    vec3 rgb = background(rd);

    if(hit) {
        vec3 n = kame_house_normal(pos);

        float lambertian = max(dot(-rd, n), 0.);

        float brightness = .5 * lambertian + .5;

        vec3 color = kame_house(pos).color;

        rgb = vec3(brightness * color);
    }

    // render_mode();

    color = vec4(rgb, 1.);
}

#include "shadertoy_suffix.fs"
