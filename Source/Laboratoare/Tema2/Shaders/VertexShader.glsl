#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Uniforms for light properties
uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;
uniform int deformation;
uniform float time;

uniform vec3 object_color;

// Output value to fragment shader
out vec3 normal;
out vec3 color;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
	// TODO: compute world space vectors
	vec3 world_pos = (Model * vec4(v_position, 1)).xyz;
	vec3 world_normal = normalize(mat3(Model) * normalize(v_normal));

	vec3 L = normalize(light_position - world_pos);
	vec3 V = normalize(eye_position - world_pos);
	vec3 H = normalize(L + V);
	vec3 R = normalize(reflect(L, world_normal));
	// TODO: define ambient light component
	float ambient_light = material_kd * 0.25;

	// TODO: compute diffuse light component
	float diffuse_light = material_kd * 1 * max(dot(world_normal, L), 0);

	// TODO: compute specular light component
	float specular_light = 0;

	if (diffuse_light > 0)
	{
		specular_light = material_ks * 1 * pow(max(dot(V, R), 0), material_shininess);
	}

	// TODO: compute light
	float attenuation_factor = 1 / (1 + 0.15 * distance(light_position, world_pos) +
							   0.075 * distance(light_position, v_position) * distance(light_position, world_pos));
	float light_intensity = 1.0 * (ambient_light + attenuation_factor * (diffuse_light + specular_light));

	// TODO: send color light output to fragment shader
	if (deformation == 2) {
		color = (object_color + vec3(cos(time), sin(time), sin(time) + cos(time))) * light_intensity;
	} else {
		color = object_color * light_intensity;
	}

	//gl_Position = Projection * View * Model * vec4(v_position, 1.0);
	if (deformation == 0 || deformation == 2) {
		gl_Position = Projection * View * Model * vec4(v_position, 1.0);
	} else if (deformation == 1) {
		vec4 v_position_deformed = Model * vec4(v_position, 1) + vec4(0, rand(vec2(v_position.x, v_position.y)), 0, 0) * 0.3;
		gl_Position = Projection * View * v_position_deformed;
	}
}
