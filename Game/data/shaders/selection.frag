#version 450
#extension GL_ARB_separate_shader_objects : enable

// OUTPUTS
layout(location = 0) out vec4 outColor;

// SET 3: Per Selection Object
layout(set = 3, binding = 0)
uniform Selection
{
	vec4 id;
};

void main() 
{
	outColor = id;
}