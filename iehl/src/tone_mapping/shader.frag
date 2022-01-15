#version 450 core

uniform sampler2D hdr_color_texture;

out vec3 f_ldr_color;

void main() {
    vec3 hdr_color = texelFetch(hdr_color_texture, ivec2(gl_FragCoord.xy), 0).rgb;
    vec3 gamma_corrected = pow(hdr_color, vec3(1. / 2.2));
    f_ldr_color = clamp(gamma_corrected, 0., 1.);
}
