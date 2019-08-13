#version 450
#extension GL_ARB_separate_shader_objects : enable

// CONSTANTS
const vec3[36] cube = vec3[] 
			 ( 
			 	vec3(-0.5, -0.5, 0.5), 
			 	vec3(0.5, -0.5, 0.5), 
			 	vec3(-0.5, 0.5, 0.5), 
			 	vec3(-0.5, 0.5, 0.5), 
			 	vec3(0.5, -0.5, 0.5), 
			 	vec3(0.5, 0.5, 0.5), 

			 	vec3(-0.5, 0.5, 0.5), 
			 	vec3(0.5, 0.5, 0.5), 
			 	vec3(-0.5, 0.5, -0.5), 
			 	vec3(-0.5, 0.5, -0.5), 
			 	vec3(0.5, 0.5, 0.5), 
			 	vec3(0.5, 0.5, -0.5), 

			 	vec3(-0.5, 0.5, -0.5), 
			 	vec3(0.5, 0.5, -0.5), 
			 	vec3(-0.5, -0.5, -0.5), 
			 	vec3(-0.5, -0.5, -0.5), 
			 	vec3(0.5, 0.5, -0.5), 
			 	vec3(0.5, -0.5, -0.5), 

			 	vec3(-0.5, -0.5, -0.5), 
			 	vec3(0.5, -0.5, -0.5), 
			 	vec3(-0.5, -0.5, 0.5), 
			 	vec3(-0.5, -0.5, 0.5), 
			 	vec3(0.5, -0.5, -0.5), 
			 	vec3(0.5, -0.5, 0.5), 

			 	vec3(0.5, -0.5, 0.5), 
			 	vec3(0.5, -0.5, -0.5), 
			 	vec3(0.5, 0.5, 0.5), 
			 	vec3(0.5, 0.5, 0.5), 
			 	vec3(0.5, -0.5, -0.5), 
			 	vec3(0.5, 0.5, -0.5), 

			 	vec3(-0.5, -0.5, -0.5), 
			 	vec3(-0.5, -0.5, 0.5), 
			 	vec3(-0.5, 0.5, -0.5), 
			 	vec3(-0.5, 0.5, -0.5), 
			 	vec3(-0.5, -0.5, 0.5), 
			 	vec3(-0.5, 0.5, 0.5) 
			 );

// OUTPUTS
layout(location = 0) out vec3 outPos;

// SET 0: Per Scene
layout(set = 0, binding = 0)
uniform Camera
{
	mat4 proj;
	mat4 view;
} camera;

void main()
{
	mat4 view = mat4(mat3(camera.view));
	gl_Position = camera.proj * view * vec4(cube[gl_VertexIndex], 1.0);
	gl_Position.w = gl_Position.z;
	outPos = cube[gl_VertexIndex];
}