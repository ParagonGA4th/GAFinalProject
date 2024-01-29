
#ifndef __DEFINED_SYSTEM_PBR_BUFFER_TEXTURE_HLSL__
#define __DEFINED_SYSTEM_PBR_BUFFER_TEXTURE_HLSL__

#include "../SamplerStates/Appends_SamplerStates.hlsli"

//¸ðµÎ RGB / A.
Texture2D<float4> t2_AlbedoAOTexture : register(t12);
Texture2D<float4> t2_NormalRoughnessTexture : register(t13);
Texture2D<float4> t2_SpecularMetallicTexture : register(t14);

float3 GetAlbedo(float2 quadUV)
{
    return t2_AlbedoAOTexture.Sample(fullScreenQuadSS, quadUV).xyz;
}

float3 GetNormal(float2 quadUV)
{
    return t2_NormalRoughnessTexture.Sample(fullScreenQuadSS, quadUV).xyz;
}

float3 GetSpecular(float2 quadUV)
{
    return t2_SpecularMetallicTexture.Sample(fullScreenQuadSS, quadUV).xyz;
}

float GetAmbientOcclusion(float2 quadUV)
{
    return t2_AlbedoAOTexture.Sample(fullScreenQuadSS, quadUV).w;
}

float GetRoughness(float2 quadUV)
{
    return t2_NormalRoughnessTexture.Sample(fullScreenQuadSS, quadUV).w;
}

float GetMetallic(float2 quadUV)
{
    return t2_SpecularMetallicTexture.Sample(fullScreenQuadSS, quadUV).w;
}

#endif //__DEFINED_SYSTEM_PBR_BUFFER_TEXTURE_HLSL__