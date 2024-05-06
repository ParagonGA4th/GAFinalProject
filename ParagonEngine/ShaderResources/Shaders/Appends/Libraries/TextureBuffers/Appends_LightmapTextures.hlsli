#ifndef __DEFINED_APPENDS_LIGHTMAP_TEXTURES_HLSL__
#define __DEFINED_APPENDS_LIGHTMAP_TEXTURES_HLSL__

#include "../SamplerStates/Appends_SamplerStates.hlsli"

//Instancing ID가 같이 들어온다.
Texture2DArray<float4> internal_LightmapArray : register(t2);

float GetLightmapDataAlpha(float2 lightmapUV, uint lightmapID)
{
    float3 tUV = float3(lightmapUV, (float)lightmapID);
    return internal_LightmapArray.Sample(lightmapSS, tUV).w;
}

float4 GetLightmapData(float2 lightmapUV, uint lightmapID)
{
    float3 tUV = float3(lightmapUV, (float) lightmapID);
    return internal_LightmapArray.Sample(lightmapSS, tUV);
}

#endif //__DEFINED_APPENDS_LIGHTMAP_TEXTURES_HLSL__