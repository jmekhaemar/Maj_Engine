#version 450
#extension GL_ARB_separate_shader_objects : enable

// OUTPUTS
layout(location = 0) out vec3 outColor;

// SET 1: Per Model
layout(set = 1, binding = 0)
uniform Model
{
	mat4 model;
	vec3 color;
};

void main() 
{
	outColor = color;
}