#version 450
#extension GL_ARB_separate_shader_objects : enable

//CONSTANTS
const vec2[4] quad = vec2[]
(
	vec2(0.0, 0.0),
	vec2(1.0, 0.0),
	vec2(0.0, 1.0),
	vec2(1.0, 1.0)
);

// OUTPUTS
layout(location = 0) out vec2 outUV;

// SET 0: Per Scene
layout(set = 0, binding = 0)
uniform Camera
{
	mat4 proj;
	mat4 view;
};

// SET 1: Per Sprite
layout(set = 1, binding = 0)
uniform Sprite
{
	vec4 position;	// w is size.x
	vec4 color;		// a is size.y
};

void main()
{
	vec2 pos = vec2(0.0, 0.0);
	
	if (gl_VertexIndex == 0)
	{
		pos.x -= position.w;
		pos.y -= color.a;
	}
	else if (gl_VertexIndex == 1)
	{
		pos.x += position.w;
		pos.y -= color.a;
	}
	else if (gl_VertexIndex == 2)
	{
		pos.x -= position.w;
		pos.y += color.a;
	}
	else
	{
		pos.x += position.w;
		pos.y += color.a;
	}

	gl_Position = proj * view * vec4(position.xyz, 1.0);
	gl_Position += vec4(pos, 0.0, 0.0);
	outUV = quad[gl_VertexIndex];
}