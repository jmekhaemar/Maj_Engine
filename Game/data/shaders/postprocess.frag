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
	//outFragColor = texColor;

	// Grey
	vec3 lumRec709 = vec3(0.2126, 0.7152, 0.0722);
	float luma = dot(texColor.rgb, lumRec709);
	outFragColor = vec4(vec3(luma), 1.0);
}