
#ifndef __DEFINED_SYSTEM_PBR_BUFFER_TEXTURE_HLSL__
#define __DEFINED_SYSTEM_PBR_BUFFER_TEXTURE_HLSL__

#include "../SamplerStates/Appends_SamplerStates.hlsli"

//모두 RGB / A. Quad-Based.
Texture2D<float4> internal_t2_AlbedoAOTexture : register(t12);
Texture2D<float4> internal_t2_NormalRoughnessTexture : register(t13);
Texture2D<float4> internal_t2_SpecularMetallicTexture : register(t14);

//IBL Texture를 쓸 수 있게 마련해둔다.
TextureCube<float4> internal_IBL_DiffuseIrrCubemap : register(t21);
TextureCube<float4> internal_IBL_SpecularIrrCubemap : register(t22);
Texture2D<float4> internal_IBL_SpecularLutTexture : register(t23);
//LUT는 여기서는 필요X!

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

float3 GetDiffuseIrradianceMap(float3 sampleUV)
{
    return internal_IBL_DiffuseIrrCubemap.Sample(defaultTextureSS, sampleUV).rgb;
}

float3 GetSpecularIrradianceMap(float3 sampleUV, float LOD)
{
    return internal_IBL_SpecularIrrCubemap.SampleLevel(defaultTextureSS, sampleUV, LOD).rgb;
}

uint IBL_querySpecularTextureLevels()
{
    uint width, height, levels;
    internal_IBL_SpecularIrrCubemap.GetDimensions(0, width, height, levels);
    return levels;
}

float2 IBL_GetSpecularBRDF(float2 sampleUV)
{
    return internal_IBL_SpecularLutTexture.Sample(defaultTextureSS, sampleUV).rg;
}
#endif //__DEFINED_SYSTEM_PBR_BUFFER_TEXTURE_HLSL__