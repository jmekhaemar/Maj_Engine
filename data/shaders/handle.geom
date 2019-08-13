#version 450
#extension GL_ARB_separate_shader_objects : enable

// INPUTS
layout(points) in;

// OUTPUTS
layout(line_strip, max_vertices = 6) out;

layout (location = 0) out vec3 outColor;

// SET 0: Per Scene
layout(set = 0, binding = 0)
uniform Camera 
{
	mat4 proj;
	mat4 view;
};

// SET 1: Per Model
layout(set = 1, binding = 0)
uniform Model 
{
	mat4 model;
};

void main()
{
	mat4 MVP = proj * view * model;
	vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);

	// X Axis

	gl_Position = MVP * origin;
	outColor = vec3(1.0, 0.0, 0.0);
	EmitVertex();
	
	gl_Position = MVP * vec4(1.0, 0.0, 0.0, 1.0);
	outColor = vec3(1.0, 0.0, 0.0);
	EmitVertex();
	
	EndPrimitive();

	// Y Axis

	gl_Position = MVP * origin;
	outColor = vec3(0.0, 1.0, 0.0);
	EmitVertex();
	
	gl_Position = MVP * vec4(0.0, 1.0, 0.0, 1.0);
	outColor = vec3(0.0, 1.0, 0.0);
	EmitVertex();
	
	EndPrimitive();
	
	// Z Axis
	
	gl_Position = MVP * origin;
	outColor = vec3(0.0, 0.0, 1.0);
	EmitVertex();

	gl_Position = MVP * vec4(0.0, 0.0, 1.0, 1.0);
	outColor = vec3(0.0, 0.0, 1.0);
	EmitVertex();
	
	EndPrimitive();
}