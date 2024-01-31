#ifndef __DEFINED_APPENDS_LIGHTMAP_TEXTURES_HLSL__
#define __DEFINED_APPENDS_LIGHTMAP_TEXTURES_HLSL__

#include "../SamplerStates/Appends_SamplerStates.hlsli"

Texture2D<float4> internal_Lightmap : register(t2);

float GetLightmapDataAlpha(float2 lightmapUV)
{
    return internal_Lightmap.Sample(lightmapSS, lightmapUV).w;
}

float4 GetLightmapData(float2 lightmapUV)
{
    return internal_Lightmap.Sample(lightmapSS, lightmapUV);
}

#endif //__DEFINED_APPENDS_LIGHTMAP_TEXTURES_HLSL__