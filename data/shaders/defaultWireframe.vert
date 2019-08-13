#version 450
#extension GL_ARB_separate_shader_objects : enable

// INPUTS
layout(location = 0) in vec3 inPos;

// SET 0: Per Scene
layout(set = 0, binding = 0)
uniform Camera 
{
	mat4 proj;
	mat4 view;
} camera;

// SET 1: Per Model
layout(set = 1, binding = 0)
uniform Model
{
	mat4 model;
	vec3 color;
};

void main()
{
	gl_Position = camera.proj * camera.view * model * vec4(inPos, 1.0);
}