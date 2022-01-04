#version 450 core

uniform sampler2D hdr_color_texture;

out vec3 f_ldr_color;

void main() {
    vec3 hdr_color = texelFetch(hdr_color_texture, ivec2(gl_FragCoord.xy), 0).rgb;
    f_ldr_color = clamp(hdr_color, 0., 1.);
}
