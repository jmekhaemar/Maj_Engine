#version 450
#extension GL_ARB_separate_shader_objects : enable

// CONSTANTS
const vec2[4] quad = vec2[]
(
	vec2(-1.0, 1.0),
	vec2(1.0, 1.0),
	vec2(-1.0, -1.0),
	vec2(1.0, -1.0)
);

// OUTPUTS
layout (location = 0) out vec2 outUV;

void main() 
{
	vec2 pos = quad[gl_VertexIndex];
	outUV = pos * 0.5 + 0.5;
	gl_Position = vec4(pos, 0.0, 1.0);
}