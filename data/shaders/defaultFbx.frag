#version 450
#extension GL_ARB_separate_shader_objects : enable

// STRUCTURES
struct Light 
{
	vec4 values;
	vec4 pos;
	vec3 color;
};

// CONSTANTS
const int MAX_LIGHT = 20;

// INPUTS
layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec2 fragUV;
layout(location = 2) in vec3 fragNormal;
layout(location = 3) in vec3 fragTangent;
layout(location = 4) in vec3 eyePos;

// OUTPUTS
layout(location = 0) out vec3 outColor;

// SET 0: Per Scene
layout(set = 0, binding = 1)
uniform allLight
{
	Light light[MAX_LIGHT];
	uint nbLight;
}lights;

// SET 1: Per Model

layout(set = 1, binding = 2)
uniform sampler2D texNormal;

layout(set = 1, binding = 3)
uniform sampler2D texSpec;

vec3 lightUpdate(uint index, vec3 N, vec3 V)
{
	vec3 L;
	if(lights.light[index].pos.w == -1.0)
		L = normalize(lights.light[index].pos.xyz - fragPos);
	else
		L = normalize(-lights.light[index].pos.xyz);

	vec3 H = normalize(L + V);

	float intensity = lights.light[index].values.w;
	if(lights.light[index].pos.w == -1.0)
	{
		float distance = length(lights.light[index].pos.xyz - fragPos);
		float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));
		intensity *= attenuation;
	}

	vec3 finalColor = lights.light[index].color * lights.light[index].values.x * intensity;
	finalColor += lights.light[index].color * clamp(max(dot(N, L), 0.0) * lights.light[index].values.y * intensity, 0.0, 1.0);
	finalColor += lights.light[index].color * clamp(pow(max(dot(N, H), 0.0), 32 * texture(texSpec, fragUV).s) * lights.light[index].values.z * intensity, 0.0, 1.0);
	return finalColor;
}

void main() 
{
	vec3 N = normalize(fragNormal);
	vec3 T = normalize(fragTangent);
	vec3 B = cross(T,N);
	mat3 TBN = mat3(T,B,N);
	
	vec3 Bump = texture(texNormal, fragUV).rgb * 2.0 - 1.0;
	N = TBN * Bump;
	
	vec3 V = normalize(eyePos - fragPos);

	vec3 finalColor = vec3(0.0, 0.0, 0.0);
	for( uint index = 0 ; index < lights.nbLight; ++index )
		finalColor += lightUpdate(index, N, V);

	outColor = finalColor;
}