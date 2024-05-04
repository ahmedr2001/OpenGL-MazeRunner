#version 330

#define DIRECTIONAL 0
#define POINT 1
#define SPOT 2

struct Light {
    int type;
    vec3 position; // for POINT and SPOT lights (to calc distance)
    vec3 direction; // for DIRECTIONAL and SPOT lights || direction of light (from light to)
    
    vec3 color;

    vec3 attenuation;
    vec2 cone_angles; // for SPOT light
};

// Defines sky color at 3 different levels
struct Sky {
    vec3 top, horizon, bottom;
};

struct Material {
    sampler2D albedo;
    sampler2D specular;
    sampler2D roughness;
    sampler2D ambient_occlusion;
    sampler2D emissive;
};

#define MAX_LIGHTS 20

uniform Light lights[MAX_LIGHTS];
uniform int light_count;
uniform Sky sky;
uniform Material material;

// input (from Vertex shader)
in Varyings {
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
    vec3 view;
    vec3 world;
} fs_in;

// output
out vec4 frag_color;

vec3 compute_sky_light(vec3 normal){
    // if normal is to top take top color, else take bottom color
    vec3 extreme = normal.y > 0 ? sky.top : sky.bottom;
    // mix the color with the horizon one
    return mix(sky.horizon, extreme, normal.y * normal.y);
}

// [Lambert's cos law] Calc dot product of DIRECTIONAL light with normal vector
float lambert(vec3 normal, vec3 world_to_light_direction) {
    // Using max(0, -ve) ensures that when the light direction and the surface normal are in opposite directions,
    //  resulting in negative illumination, the value is clamped to zero to signify no light.
    return max(0.0, dot(normal, world_to_light_direction));
}

// Calc phong specular formula
float phong(vec3 reflected, vec3 view, float shininess) {
    return pow(max(0.0, dot(reflected, view)), shininess);
}

void main() {
    vec3 normal = normalize(fs_in.normal); // normal vector
    vec3 normal = normalize(fs_in.normal); // normal vector
    vec3 view = normalize(fs_in.view);

    // Sky light effect 
    vec3 ambient_light = compute_sky_light(normal);
    
    // Material textures 
    vec3 diffuse = texture(material.albedo, fs_in.tex_coord).rgb;
    vec3 specular = texture(material.specular, fs_in.tex_coord).rgb;
    vec3 ambient = diffuse * texture(material.ambient_occlusion, fs_in.tex_coord).r;
    
    float roughness = texture(material.roughness, fs_in.tex_coord).r;
    vec3 emissive = texture(material.emissive, fs_in.tex_coord).rgb;

    float shininess = 2.0 / pow(clamp(roughness, 0.001, 0.999), 4.0) - 2.0;
    
    // 
    vec3 world_to_light_dir;
    vec3 color = emissive + ambient;
    // vec3 color = emissive + ambient_light * ambient; // uncomment if we want sky light

    // iterate over lights
    int loop_lights = min(MAX_LIGHTS, light_count);
    for(int light_idx = 0; light_idx < loop_lights; light_idx++) {
        Light light = lights[light_idx];
        float attenuation = 1.0;

        if(light.type == DIRECTIONAL) // DIRECTIONAL
        {
            // Light direction is same for any point in the world
			world_to_light_dir = -light.direction;
        } else { // SPOT || POINT 
            // Light direction differs from point to point in the world
            world_to_light_dir = normalize(light.position - fs_in.world);
            float distance_to_light = length(light.position - fs_in.world);

            attenuation = 1.0 / dot(light.attenuation, vec3(distance_to_light*distance_to_light, distance_to_light, 1.0)); // Attenuation (1,0,0) decreases with ( d^2 ), (0,1,0) diminishes linearly with ( d ), (0,0,1) and remains constant otherwise.
            if(light.type == SPOT){
                float angle = acos(dot(light.direction, -world_to_light_dir)); // get angle between light_vector and world_to_light vector
                attenuation *= smoothstep(light.cone_angles.y, light.cone_angles.x, angle); // change attenuation from outer angle to inner angle moving by step angle
                float angle = acos(dot(light.direction, -world_to_light_dir)); // get angle between light_vector and world_to_light vector
                attenuation *= smoothstep(light.cone_angles.y, light.cone_angles.x, angle); // change attenuation from outer angle to inner angle moving by step angle
            }
        }

        vec3 computed_diffuse = light.color * diffuse * lambert(normal, world_to_light_dir);
        vec3 reflected = reflect(-world_to_light_dir, normal);

        vec3 computed_specular = light.color * specular * phong(reflected, view, shininess);

        color += (computed_diffuse + computed_specular) * attenuation;
    }

    // set ouput 
    frag_color = vec4(color, 1.0);
}