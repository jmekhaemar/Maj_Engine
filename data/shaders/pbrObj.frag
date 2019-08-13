#version 450
#extension GL_ARB_separate_shader_objects : enable

// STRUCTURES
struct Light 
{
	vec4 values;
	vec4 pos;
	vec3 color;
};

struct PBRInfo
{
	vec3 N;
	vec3 V;
	vec3 albedo;
	float metallic;
	float roughness;
	float ao;

	vec3 F0;
	float NdotV;
};

// CONSTANTS
const int MAX_LIGHT = 20;
const float BASE_REFLECTIVITY = 0.04;
const float PI = 3.14159265359;

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

layout(set = 0, binding = 2)
uniform samplerCube texEnvIrradiance;
layout(set = 0, binding = 3)
uniform samplerCube texEnv;
layout(set = 0, binding = 4)
uniform sampler2D texBRDFLUT;

// SET 1: Per Model
layout(set = 1, binding = 1)
uniform MaterialInfo
{
	vec3 albedo;
	float metallic;
	float roughness;
} mat;

// Utility functions
float dotClamped(vec3 a, vec3 b);
vec3 correctGamma(vec3 color);

vec3 processLight(int index, PBRInfo info);

vec3 getIncomingLightDir(int index);
float getIncomingLightIntensity(int index);
float computeAttenuation(float sqrDistance);

// Base reflectivity of a surface at incident angle of 0°
// returns how reflective is when hit directly in the face
vec3 computeBaseReflectivity(vec3 albedo, float metallic);

// Energy conservation principle
// returns the diffuse component, according to the energy spent on the specular
// reflected and refracted light must never be greater than incoming light
vec3 computeEnergyConservation(vec3 kS, float metallic);

// GGX normal distribution function (Trowbidge-Reitz)
// returns how aligned the microfacets are to the halfway vector
float computeGGXDistribution(vec3 N, vec3 H, float roughness);

// Smith geometry function using GGX geometry with Schlick's approximation
// returns how much the microfacets shadow themselves
// Smith algorithm splits in two, one for the light and one for the view
// (usually takes N, V, L as parameters, here we avoid recalculation)
float computeSmithGeometry(float NdotV, float NdotL, float roughness);
float computeGGXGeometrySchlick(float cosTheta, float roughness);

// Fresnel equation with Schlick's approximation
// returns the ratio of reflected light over refracted light
// cosTheta = angle between the view direction and the halfway vector
// F0 = base reflectivity at incident angle of 0°
vec3 fresnelSchlick(float cosTheta, vec3 F0);
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness);

// Image-based lighting
// calculates the diffuse and specular from indirect lighting
vec3 calculateIBL(PBRInfo info);

// Sample the environment BRDF LUT
vec2 getEnvironmentBRDF(float NdotV, float roughness);

void main() 
{
	// Get material & geometry properties
	PBRInfo info;
	info.metallic = mat.metallic;
	info.roughness = mat.roughness;
	info.albedo = mat.albedo;
	info.ao = 1.0;
	
	info.F0 = computeBaseReflectivity(info.albedo, info.metallic);

	info.N = normalize(fragNormal);
	info.V = normalize(eyePos - fragPos);
	info.NdotV = abs(dot(info.N, info.V)); // Absolute dot instead of clamping (Unity negative dot fix)

	// Calculate lights
	vec3 irradiance = vec3(0.0, 0.0, 0.0);		// Irradiance is the sum of all radiance (incomming light)
	for(int index = 0 ; index < lights.nbLight; ++index)
	{
		irradiance += processLight(index, info);
	}
	
	// Apply gamma-corrected direct & indirect lighting
	outColor = correctGamma(irradiance + calculateIBL(info));
}

float dotClamped(vec3 a, vec3 b)
{
	// No need to clamp to 1.0 because unit vectors
	return max(dot(a, b), 0.0);
}

vec3 correctGamma(vec3 color)
{
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));
	return color;
}

vec3 processLight(int index, PBRInfo info)
{
	// Calculate radiance
	vec3 L = getIncomingLightDir(index);
	vec3 H = normalize(info.V + L);
	float intensity = getIncomingLightIntensity(index);
	vec3 radiance = lights.light[index].color * intensity;

	// Normal Distribution Function = approximates how much the microfacets are aligned to the halfway vector
	float NDF = computeGGXDistribution(info.N, H, info.roughness);

	// Geometry function = describes self-shadowing of microfacets
	float NdotL = dotClamped(L, info.N);
	float G = computeSmithGeometry(info.NdotV, NdotL, info.roughness);

	// Fresnel equation = describes surface reflection
	vec3 F = fresnelSchlick(dotClamped(H, info.V), info.F0);

	// Apply energy conservation
	vec3 diffuse = computeEnergyConservation(F, info.metallic);
	
	// Calculate Cook-Torrance BDRF
	vec3 CTnumerator = NDF * G * F;								// Cook-Torrance numerator
	float CTdenominator = max(4.0 * info.NdotV * NdotL, 0.001);	// Cook-Torrance denominator (avoid division by 0)
	vec3 specular = CTnumerator / CTdenominator;

	return (diffuse * info.albedo / PI + specular) * radiance * NdotL;
}

vec3 getIncomingLightDir(int index)
{
	if(lights.light[index].pos.w == -1.0)
		return normalize(lights.light[index].pos.xyz - fragPos);
	else
		return normalize(-lights.light[index].pos.xyz);
}

float getIncomingLightIntensity(int index)
{
	float intensity = lights.light[index].values.w;
	if(lights.light[index].pos.w == -1.0)	// Apply attenuation
	{
		vec3 toLight = lights.light[index].pos.xyz - fragPos;
		float sqrDistance = abs(dot(toLight, toLight));
		intensity *= computeAttenuation(sqrDistance);
	}

	return intensity;
}

float computeAttenuation(float sqrDistance)
{
	return 1.0 / sqrDistance;
}

vec3 computeBaseReflectivity(vec3 albedo, float metallic)
{
	vec3 F0 = vec3(BASE_REFLECTIVITY);
	return mix(F0, albedo, metallic);
}

vec3 computeEnergyConservation(vec3 kS, float metallic)
{
	// metallic materials are supposed to have no diffuse
	return (1.0 - kS) * (1.0 - metallic);
}

float computeGGXDistribution(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    a *= a;

    float NdotH  = dotClamped(N, H);
    NdotH *= NdotH;
	
    float denominator = NdotH * (a - 1.0) + 1.0;
    denominator *= PI * denominator;
	
    return a / denominator;
}

float computeSmithGeometry(float NdotV, float NdotL, float roughness)
{
    float ggx2 = computeGGXGeometrySchlick(NdotV, roughness);
    float ggx1 = computeGGXGeometrySchlick(NdotL, roughness);
	
    return ggx2 * ggx1;
}

float computeGGXGeometrySchlick(float cosTheta, float roughness)
{
	float r = roughness * roughness;
	float k = r * 0.5;
	
    float denominator = cosTheta * (1.0 - k) + k;
    return cosTheta / max(denominator, 0.001);
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
	return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}

vec3 calculateIBL(PBRInfo info)
{
	vec3 F = fresnelSchlickRoughness(info.NdotV, info.F0, info.roughness);
	vec3 diffuse = computeEnergyConservation(F, info.metallic);
	
	vec3 R = reflect(-info.V, info.N);
	vec3 irradiance = texture(texEnvIrradiance, R).rgb;
	
	float lod = info.roughness * (textureQueryLevels(texEnv) - 1);
	vec3 reflection = texture(texEnv, R, lod).rgb;
	vec2 envBRDF = getEnvironmentBRDF(info.NdotV, info.roughness);
	
	vec3 total = diffuse * irradiance * info.albedo + reflection * (F * envBRDF.x + envBRDF.y);
	return total * info.ao;
}

vec2 getEnvironmentBRDF(float NdotV, float roughness)
{
	return texture(texBRDFLUT, vec2(NdotV, roughness)).xy;
}