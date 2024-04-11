
#ifndef __DEFINED_APPENDS_MATH_FUNCTIONS_PS_HLSL__
#define __DEFINED_APPENDS_MATH_FUNCTIONS_PS_HLSL__

//<Defines>
static const float PI = 3.14159265359;
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

//언리얼 구현체 참조.
half3 sRGBToLinear(half3 Color)
{
    Color = max(6.10352e-5, Color); // minimum positive non-denormal (fixes black problem on DX11 AMD and NV)
    return Color > 0.04045 ? pow(Color * (1.0 / 1.055) + 0.0521327, 2.4) : Color * (1.0 / 12.92);
}

//PBR cginc 기반.
float fPow5(float v)
{
    return pow(1 - v, 5);
}

// Diffuse distribution functions

float3 lambertDiffuse(float3 albedo)
{
    return albedo / PI;
}

// Fresnel functions

float3 fresnel(float3 F0, float NdotV)
{
    return F0 + (1 - F0) * fPow5(NdotV);
}

float3 fresnel(float3 F0, float NdotV, float roughness)
{
    return F0 + (max(1.0 - roughness, F0) - F0) * fPow5(NdotV);
}

float3 fresnelDisney(float HdotL, float NdotL, float NdotV, float roughness)
{
    float k = 0.5 + 2 * roughness * sqrt(HdotL);
    float firstTerm = (k - 1) * fPow5(NdotV) + 1;
    float secondTerm = (k - 1) * fPow5(NdotL) + 1;
    return firstTerm * secondTerm;
}

float3 F0(float ior)
{
    return pow((1.0 - ior) / (1.0 + ior), 2);
}

// Normal distribution functions

float trowbridgeReitzNDF(float NdotH, float roughness)
{
    float alpha = roughness * roughness;
    float alpha2 = alpha * alpha;
    float NdotH2 = NdotH * NdotH;
    float denominator = PI * pow((alpha2 - 1) * NdotH2 + 1, 2);
    return alpha2 / denominator;
}

float trowbridgeReitzAnisotropicNDF(float NdotH, float roughness, float anisotropy, float HdotT, float HdotB)
{
    float aspect = sqrt(1.0 - 0.9 * anisotropy);
    float alpha = roughness * roughness;

    float roughT = alpha / aspect;
    float roughB = alpha * aspect;

    float alpha2 = alpha * alpha;
    float NdotH2 = NdotH * NdotH;
    float HdotT2 = HdotT * HdotT;
    float HdotB2 = HdotB * HdotB;

    float denominator = PI * roughT * roughB * pow(HdotT2 / (roughT * roughT) + HdotB2 / (roughB * roughB) + NdotH2, 2);
    return 1 / denominator;
}

// Geometric attenuation functions

float cookTorranceGAF(float NdotH, float NdotV, float HdotV, float NdotL)
{
    float firstTerm = 2 * NdotH * NdotV / HdotV;
    float secondTerm = 2 * NdotH * NdotL / HdotV;
    return min(1, min(firstTerm, secondTerm));
}

float schlickBeckmannGAF(float dotProduct, float roughness)
{
    float alpha = roughness * roughness;
    float k = alpha * 0.797884560803; // sqrt(2 / PI)
    return dotProduct / (dotProduct * (1 - k) + k);
}

// Helpers
float3 gammaCorrection(float3 v)
{
    return pow(v, 1.0 / 2.2);
}

float3 sRGB2Lin(float3 col)
{
    return pow(col, 2.2);
}

#endif // __DEFINED_APPENDS_MATH_FUNCTIONS_PS_HLSL__