#version 450
#extension GL_ARB_separate_shader_objects : enable

// CONSTANTS
const int wireIteration = 20;
const int wireSize = 10;

// INPUTS
layout(points) in;

// OUTPUTS
layout(line_strip, max_vertices = wireIteration * 4 * 2) out;

// SET 0: Per Scene
layout(set = 0, binding = 0)
uniform Camera 
{
	mat4 proj;
	mat4 view;
} camera;

void main()
{
	mat4 PV = camera.proj * camera.view;

	for(int i = -wireIteration + 1; i < wireIteration; ++i)
	{
		gl_Position = PV * vec4(-wireIteration * wireSize, 0.0, i * wireSize, 1.0);
		EmitVertex();

		gl_Position = PV * vec4(wireIteration * wireSize, 0.0, i * wireSize, 1.0);
		EmitVertex();
		
		EndPrimitive();

		gl_Position = PV * vec4(i * wireSize, 0.0, -wireIteration * wireSize, 1.0);
		EmitVertex();

		gl_Position = PV * vec4(i * wireSize, 0.0, wireIteration * wireSize, 1.0);
		EmitVertex();

		EndPrimitive();
	}
}