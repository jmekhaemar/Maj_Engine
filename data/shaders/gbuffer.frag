#version 450
#extension GL_ARB_separate_shader_objects : enable

// INPUTS
layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec2 fragUV;
layout(location = 2) in vec3 fragNormal;
layout(location = 3) in vec3 fragTangent;
layout(location = 4) in vec3 eyePos;

// OUTPUTS
layout(location = 0) out vec4 outPosition;
layout(location = 1) out vec4 outNormal;
layout(location = 2) out vec4 outMaterial; // Roughness, Specular, Metallic, etc
layout(location = 3) out vec3 outDiffuse;

// SET 0: Per Scene

// SET 1: Per Model
layout(set = 1, binding = 1)
uniform sampler2D texSampler;

layout(set = 1, binding = 2)
uniform sampler2D texNormal;

layout(set = 1, binding = 3)
uniform sampler2D texSpec;

void main() 
{
	// Position
	outPosition.xyz = fragPos;

	// Normal
	vec3 N = normalize(fragNormal);
	vec3 T = normalize(fragTangent);
	vec3 B = cross(T,N);
	mat3 TBN = mat3(T,B,N);
	
	vec3 Bump = texture(texNormal, fragUV).rgb * 2.0 - 1.0;
	outNormal.xyz = TBN * Bump;
	
	// Material properties
	outMaterial.rgb = texture(texSpec, fragUV).rgb;
	
	// Diffuse
	outDiffuse.rgb = texture(texSampler, fragUV).rgb;
	
	// View direction
	vec3 V = normalize(eyePos - fragPos);
	outPosition.w = V.x;
	outNormal.w = V.y;
	outMaterial.w= V.z;
}