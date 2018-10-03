#version 450
#extension GL_ARB_separate_shader_objects : enable

// INPUTS
layout (location = 0) in vec2 inUV;

// OUTPUTS
layout (location = 0) out vec4 outFragColor;

// SET 0: Per Frame
layout (input_attachment_index = 0, set = 0, binding = 0) uniform subpassInput samplerColor;

void main() 
{
	vec4 texColor = subpassLoad(samplerColor);
	
	// Original
	outFragColor = texColor;
}