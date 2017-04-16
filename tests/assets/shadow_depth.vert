#version 330 core

layout (location = 0) in vec3 in_position;

uniform mat4 light_space;
uniform mat4 model;

void main() {
    gl_Position = light_space * model * vec4(in_position, 1.0);
}