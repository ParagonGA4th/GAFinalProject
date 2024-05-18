
#ifndef __DEFINED_SYSTEM_PBR_BUFFER_TEXTURE_HLSL__
#define __DEFINED_SYSTEM_PBR_BUFFER_TEXTURE_HLSL__

#include "../SamplerStates/Appends_SamplerStates.hlsli"
#include "../Required/Appends_BufferSRVRegisters.hlsli"

//IBL Texture를 쓸 수 있게 마련해둔다.
TextureCube<float4> internal_IBL_DiffuseIrrCubemap : register(t20);
TextureCube<float4> internal_IBL_SpecularIrrCubemap : register(t21);
Texture2D<float4> internal_IBL_SpecularLutTexture : register(t22);
//LUT는 여기서는 필요X!

float GetAmbientOcclusionMap(float2 quadUV)
{
    return internal_t2_ObjMatAoR.Sample(defaultTextureSS, quadUV).z;
}

float GetRoughnessMap(float2 quadUV)
{
    return internal_t2_ObjMatAoR.Sample(defaultTextureSS, quadUV).w;
}

float3 GetAlbedoMap(float2 quadUV)
{
    return internal_t2_AlbedoMetallic.Sample(defaultTextureSS, quadUV).xyz;
}

float GetMetallicMap(float2 quadUV)
{
    return internal_t2_AlbedoMetallic.Sample(defaultTextureSS, quadUV).w;
}

float3 GetNormalMap(float2 quadUV)
{
    return internal_t2_NormalAlpha.Sample(defaultTextureSS, quadUV).xyz;
}

float GetAlphaMap(float2 quadUV)
{
    return internal_t2_NormalAlpha.Sample(defaultTextureSS, quadUV).w;
}

float3 GetDiffuseIrradianceMap(float3 sampleUV)
{
    //덜 상세하게 나오지만, 상관X.
    //return internal_IBL_DiffuseIrrCubemap.Sample(defaultTextureSS, sampleUV).rgb;
    
    return internal_IBL_DiffuseIrrCubemap.Sample(fullScreenQuadSS, sampleUV).rgb;
}

float3 GetSpecularIrradianceMap(float3 sampleUV, float LOD)
{
     //덜 상세하게 나오지만, 상관X.
    //return internal_IBL_SpecularIrrCubemap.SampleLevel(defaultTextureSS, sampleUV, LOD).rgb;
    return internal_IBL_SpecularIrrCubemap.SampleLevel(fullScreenQuadSS, sampleUV, LOD).rgb;
}

float2 IBL_GetSpecularBRDF(float2 sampleUV)
{
    //return internal_IBL_SpecularLutTexture.Sample(defaultTextureSS, sampleUV).rg;
    return internal_IBL_SpecularLutTexture.Sample(fullScreenQuadSS, sampleUV).rg;
}

uint IBL_querySpecularTextureLevels()
{
    uint width, height, levels;
    internal_IBL_SpecularIrrCubemap.GetDimensions(0, width, height, levels);
    return levels;
}


#endif //__DEFINED_SYSTEM_PBR_BUFFER_TEXTURE_HLSL__