#version 330 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec3 normal;
out vec3 fragment_position;

void main() {
    normal = mat3(transpose(inverse(model))) * in_normal;  
    gl_Position = projection * view * model * vec4(in_position, 1.0);
    fragment_position = vec3(model * vec4(in_position, 1.0));
}