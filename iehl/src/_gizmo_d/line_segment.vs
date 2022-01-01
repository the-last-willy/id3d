#version 450 core

uniform vec3 position0;
uniform vec3 position1;
uniform mat4 transform = mat4(
    1., 0., 0., 0.,
    0., 1., 0., 0.,
    0., 0., 1., 0.,
    0., 0., 0., 1.);

void main() {
    if(gl_VertexID == 0) {
        gl_Position = transform * vec4(position0, 1.);
    } else {
        gl_Position = transform * vec4(position1, 1.);
    }
}
