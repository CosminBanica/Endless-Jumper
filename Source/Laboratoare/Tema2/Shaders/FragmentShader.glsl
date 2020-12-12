#version 330

// TODO: get color value from vertex shader
in vec3 normal;
in vec3 color;

layout(location = 0) out vec4 out_color;
layout(location = 1) out vec4 out_normal;

void main()
{
	// TODO: write pixel out color
	out_color = vec4(color, 1);
	out_normal = vec4(abs(normal), 1);
}