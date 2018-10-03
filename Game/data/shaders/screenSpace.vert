#version 450
#extension GL_ARB_separate_shader_objects : enable

// CONSTANTS
const vec2[4] quad = vec2[]
(
	vec2(0.0, 0.0),
	vec2(1.0, 0.0),
	vec2(0.0, 1.0),
	vec2(1.0, 1.0)
);

// OUTPUTS
layout(location = 0) out vec2 outUV;

// SET 2: Per UI Element
layout(set = 2, binding = 0)
uniform UIElement
{
	vec2 position;
	vec2 size;
	vec4 color;
};

void main()
{
	vec2 pos = vec2(-1.0, -1.0) + position * 2.0;
	
	if (gl_VertexIndex == 1 || gl_VertexIndex == 3)
		pos.x += size.x * 2.0;
	
	if (gl_VertexIndex == 2 || gl_VertexIndex == 3)
		pos.y += size.y * 2.0;

	gl_Position = vec4(pos, 0.0, 1.0);
	outUV = quad[gl_VertexIndex];
}