#version 330 core

const vec3 light_color = vec3(1.0, 1.0, 1.0);
const float ambient_strength = 0.1;
const float specular_strength = 0.5;

const float greyscale = 1.0;
const vec3 object_color = vec3(greyscale, greyscale, greyscale);

in vec3 normal;
in vec3 fragment_position;

uniform vec3 light_position;
uniform vec3 view_position;

out vec4 color;

void main() {

    vec3 ambient = light_color * ambient_strength;

    vec3 norm = normalize(normal);
    vec3 light_direction = normalize(light_position - fragment_position);
    float diff = max(dot(norm, light_direction), 0.0);
    vec3 diffuse = light_color * diff;

    vec3 view_direction = normalize(view_position - fragment_position);
    vec3 reflect_direction = reflect(-view_direction, norm);
    float spec = pow(max(dot(reflect_direction, view_direction), 0.0), 64);
    vec3 specular = light_color * spec;

    vec3 result = (ambient + diffuse + specular) * object_color;
    color = vec4(result, 1.0);

}