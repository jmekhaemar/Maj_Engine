#version 450
#extension GL_ARB_separate_shader_objects : enable

// CONSTANTS
const float alphaCutOff = 0.1;

// INPUTS
layout(location = 0) in vec2 fragUV;

// OUTPUTS
layout(location = 0) out vec4 outColor;

// SET 1: Per Sprite
layout(set = 1, binding = 0)
uniform Sprite
{
	vec4 position;	// w is size.x
	vec4 color;		// a is size.y
};

layout(set = 1, binding = 1)
uniform sampler2D texSampler;

void main()
{
	outColor = texture(texSampler, fragUV) * vec4(color.rgb, 1.0);
	if (outColor.a < alphaCutOff)
		discard;
}