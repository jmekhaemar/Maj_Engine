#version 450
#extension GL_ARB_separate_shader_objects : enable

// CONSTANTS
const float alphaCutOff = 0.1;

// INPUTS
layout(location = 0) in vec2 fragUV;

// OUTPUTS
layout(location = 0) out vec4 outColor;

// SET 2: Per UI Element
layout(set = 2, binding = 0)
uniform UIElement
{
	vec2 position;
	vec2 size;
	vec4 color;
};

layout(set = 2, binding = 1)
uniform sampler2D texSampler;

void main()
{
	outColor = texture(texSampler, fragUV) * color;
	if (outColor.a < alphaCutOff)
		discard;
}