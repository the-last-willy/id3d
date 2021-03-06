#version 450 core

vec2 positions[6] = vec2[6](
    vec2(-1., -1.), vec2( 1., -1.), vec2(-1.,  1.),
    vec2(-1.,  1.), vec2( 1., -1.), vec2( 1.,  1.));

void main() {
    gl_Position = vec4(positions[gl_VertexID], 0., 1.);
}
