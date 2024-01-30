
#ifndef __DEFINED_SYSTEM_PBR_BUFFER_TEXTURE_HLSL__
#define __DEFINED_SYSTEM_PBR_BUFFER_TEXTURE_HLSL__

#include "../SamplerStates/Appends_SamplerStates.hlsli"

//¸ðµÎ RGB / A. Quad-Based.
Texture2D<float4> internal_t2_AlbedoAOTexture : register(t12);
Texture2D<float4> internal_t2_NormalRoughnessTexture : register(t13);
Texture2D<float4> internal_t2_SpecularMetallicTexture : register(t14);

float3 GetAlbedoMap(float2 quadUV)
{
    return internal_t2_AlbedoAOTexture.Sample(fullScreenQuadSS, quadUV).xyz;
}

float3 GetNormalMap(float2 quadUV)
{
    return internal_t2_NormalRoughnessTexture.Sample(fullScreenQuadSS, quadUV).xyz;
}

float3 GetSpecularMap(float2 quadUV)
{
    return internal_t2_SpecularMetallicTexture.Sample(fullScreenQuadSS, quadUV).xyz;
}

float GetAmbientOcclusionMap(float2 quadUV)
{
    return internal_t2_AlbedoAOTexture.Sample(fullScreenQuadSS, quadUV).w;
}

float GetRoughnessMap(float2 quadUV)
{
    return internal_t2_NormalRoughnessTexture.Sample(fullScreenQuadSS, quadUV).w;
}

float GetMetallicMap(float2 quadUV)
{
    return internal_t2_SpecularMetallicTexture.Sample(fullScreenQuadSS, quadUV).w;
}

#endif //__DEFINED_SYSTEM_PBR_BUFFER_TEXTURE_HLSL__