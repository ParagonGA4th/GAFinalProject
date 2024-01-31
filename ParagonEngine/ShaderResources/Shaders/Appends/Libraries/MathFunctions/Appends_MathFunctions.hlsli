
#ifndef __DEFINED_APPENDS_MATH_FUNCTIONS_PS_HLSL__
#define __DEFINED_APPENDS_MATH_FUNCTIONS_PS_HLSL__

//<Defines>
static const float PI = 3.141592;
static const float Epsilon = 0.00001;

// Constant normal incidence Fresnel factor for all dielectrics.
static const float3 Fdielectric = 0.04;
//</Defines>


float3 NormalSampleToWorldSpace(float3 normalMapSample, float3 unitNormalW, float3 tangentW)
{
    float3 normalT = 2.0f * normalMapSample - 1.0f; // 0-1에서 -1-1로.
	
	//정규직교 기저.
    float3 N = unitNormalW; // Normal
    float3 T = normalize(tangentW - dot(tangentW, N) * N); // Tangent
    float3 B = cross(N, T); // Binormal
	
    float3x3 TBN = float3x3(T, B, N);
	
    float3 bumpedNormalW = mul(normalT, TBN);
    return bumpedNormalW;
}

// GGX/Towbridge-Reitz normal distribution function.
// Uses Disney's reparametrization of alpha = roughness^2.
float PBR_ndfGGX(float cosLh, float roughness)
{
    float alpha = roughness * roughness;
    float alphaSq = alpha * alpha;

    float denom = (cosLh * cosLh) * (alphaSq - 1.0) + 1.0;
    return alphaSq / (PI * denom * denom);
}

// Single term for separable Schlick-GGX below.
float PBR_gaSchlickG1(float cosTheta, float k)
{
    return cosTheta / (cosTheta * (1.0 - k) + k);
}

// Schlick-GGX approximation of geometric attenuation function using Smith's method.
float PBR_gaSchlickGGX(float cosLi, float cosLo, float roughness)
{
    float r = roughness + 1.0;
    float k = (r * r) / 8.0; // Epic suggests using this roughness remapping for analytic lights.
    return PBR_gaSchlickG1(cosLi, k) * PBR_gaSchlickG1(cosLo, k);
}

// Shlick's approximation of the Fresnel factor.
float3 PBR_fresnelSchlick(float3 F0, float cosTheta)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

#endif // __DEFINED_APPENDS_MATH_FUNCTIONS_PS_HLSL__