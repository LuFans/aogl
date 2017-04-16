#version 330 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 light_space;

out vec3 normal;
out vec3 fragment_position;
out vec4 frag_light_space;

void main() {
    normal = mat3(transpose(inverse(model))) * in_normal;  
    gl_Position = projection * view * model * vec4(in_position, 1.0);
    fragment_position = vec3(model * vec4(in_position, 1.0));
    frag_light_space = light_space * vec4(fragment_position, 1.0);
}