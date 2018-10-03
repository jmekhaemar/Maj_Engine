#version 450
#extension GL_ARB_separate_shader_objects : enable

// INPUTS
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec3 inTangent;

// OUTPUTS
layout(location = 0) out vec3 outPos;
layout(location = 1) out vec2 outUV;
layout(location = 2) out vec3 outNormal;
layout(location = 3) out vec3 outTangent;
layout(location = 4) out vec3 eyePos;

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
};

void main()
{
	gl_Position = camera.proj * camera.view * model * vec4(inPos, 1.0);
	outPos = vec3(model * vec4(inPos, 1.0));
	outUV = inUV;
	outNormal = normalize(transpose(inverse(mat3(model))) * inNormal);
	outTangent = normalize(transpose(inverse(mat3(model))) * inTangent);
	eyePos = -camera.view[3].xyz;
}