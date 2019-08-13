#version 450
#extension GL_ARB_separate_shader_objects : enable

// INPUTS
layout(location = 0) in vec3 inPos; 

// OUTPUTS
layout(location = 0) out vec4 color;

// SET 1: Per Cubemap
layout(set = 1, binding = 0)
uniform samplerCube texSampler;

void main()
{
	color = texture(texSampler, inPos);
}