#version 330 core

const vec3 light_color = vec3(1.0, 1.0, 1.0);
const float ambient_strength = 0.1;
const float specular_strength = 0.5;

const float greyscale = 1.0;
const vec3 object_color = vec3(89.0 / 255.0, 86.0 / 255.0, 82.0 / 255.0);

in vec3 normal;
in vec3 fragment_position;
in vec4 frag_light_space;

uniform vec3 light_position;
uniform vec3 view_position;

uniform sampler2D depth_map;

out vec4 color;

float calculate_shadow(vec4 frag_position_light_space, vec3 normal, vec3 light_direction) {
    vec3 projection_coordinates = frag_position_light_space.xyz / frag_position_light_space.w;
    projection_coordinates = projection_coordinates * 0.5 + 0.5;

    if (projection_coordinates.z > 1.0) 
        return 0.0;

    float current_depth = projection_coordinates.z;

    float bias = max(0.05 * (1.0 - dot(normal, light_direction)), 0.005);  

    float shadow = 0.0;
    vec2 texel_size = 1.0 / textureSize(depth_map, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcf_depth = texture(depth_map, projection_coordinates.xy + vec2(x, y) * texel_size).r; 
            shadow += current_depth - bias > pcf_depth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

    return shadow;
}

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

    float shadow = calculate_shadow(frag_light_space, norm, light_direction);

    vec3 result = (ambient + (1.0 - shadow) * (diffuse + specular)) * object_color;
    color = vec4(result, 1.0);

}