#version 450
#extension GL_ARB_separate_shader_objects : enable

// CONSTANTS
const int MAX_BONES = 100;

// INPUTS
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec3 inNormal;
layout(location = 3) in vec3 inTangent;
layout(location = 4) in vec4 inBoneIndex;
layout(location = 5) in vec4 inBoneWeight;

// OUTPUTS
layout(location = 0) out vec3 outPos;
layout(location = 1) out vec2 outUV;
layout(location = 2) out vec3 outNormal;
layout(location = 3) out vec3 outTangent;
layout(location = 4) out vec3 eyePos;

// SET 0: Per Scene
layout(set = 0, binding = 0)
uniform Camera 
{
	mat4 proj;
	mat4 view;
} camera;

// SET 1: Per Model
layout(set = 1, binding = 0)
uniform Model
{
	mat4 model;
	mat4 skin[MAX_BONES];
	int nbBone;
};

void main()
{
	vec4 pos4 = vec4(inPos, 1.0f);
	vec4 pos = vec4(0, 0, 0, 0);
	vec3 nor = vec3(0, 0, 0);
	vec3 tan = vec3(0, 0, 0);
	
	if(inBoneWeight[0] + inBoneWeight[1] + inBoneWeight[2] + inBoneWeight[3] == 0)
	{
		pos = pos4;
		nor = inNormal;	
		tan = inTangent;
	}
	else if(nbBone < MAX_BONES)
	{
		mat4 m;
		for(int i = 0 ; i < 4 ; ++i)
		{
			m = skin[int(inBoneIndex[i])];
			pos += (m * pos4) * inBoneWeight[i];
			nor += (mat3(m) * inNormal) * inBoneWeight[i];
			tan += (mat3(m) * inTangent) * inBoneWeight[i];
		}
	}
	
	gl_Position = camera.proj * camera.view * model * pos;
	outPos = vec3(model * pos);
	outUV = inUV;
	outNormal = normalize(transpose(inverse(mat3(model))) * nor);
	outTangent = normalize(transpose(inverse(mat3(model))) * tan);
	eyePos = -inverse(camera.view)[3].xyz;
}