#version 450
#extension GL_ARB_separate_shader_objects : enable

// INPUTS
layout (location = 0) in vec2 inUV;

// OUTPUTS
layout (location = 0) out vec4 outFragColor;

// SET 0: Per Frame
layout (input_attachment_index = 0, set = 0, binding = 0) uniform subpassInput samplerAlbedo;
layout (input_attachment_index = 1, set = 0, binding = 1) uniform subpassInput samplerPosition;
layout (input_attachment_index = 2, set = 0, binding = 2) uniform subpassInput samplerNormal;
layout (input_attachment_index = 3, set = 0, binding = 3) uniform subpassInput samplerMaterial;

// SET 1: Per Light
layout(set = 1, binding = 0)
uniform Light
{
	vec4 values;
	vec4 pos;
	vec3 color;
};

void main() 
{
	vec3 albedo = subpassLoad(samplerAlbedo).rgb;
	vec4 P = subpassLoad(samplerPosition);
	vec4 N = subpassLoad(samplerNormal);
	vec4 material = subpassLoad(samplerMaterial);
	
	vec3 V = vec3(P.w, N.w, material.w);
	
	vec3 L;
	if(pos.w == -1.0)
		L = normalize(pos.xyz - P.xyz);
	else
		L = normalize(-pos.xyz);

	vec3 H = normalize(L + V);

	float intensity = values.w;
	if(pos.w == -1.0)
	{
		float distance = length(pos.xyz - P.xyz);
		float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));
		intensity *= attenuation;
	}

	vec3 finalColor = color * values.x * intensity;
	finalColor += color * clamp(max(dot(N.xyz, L), 0.0) * values.y * intensity, 0.0, 1.0);
	finalColor += color * clamp(pow(max(dot(N.xyz, H), 0.0), 32 * material.x) * values.z * intensity, 0.0, 1.0);
	
	outFragColor = vec4(albedo * finalColor, 1.0);
}
