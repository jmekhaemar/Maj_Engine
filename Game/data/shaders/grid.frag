#version 450
#extension GL_ARB_separate_shader_objects : enable

// OUTPUTS
layout(location = 0) out vec4 outColor;

void main()
{
	outColor = vec4(0.3, 0.3, 0.3, 1.0);
}