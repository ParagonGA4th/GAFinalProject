///Quad에서 전달되는 기본적인 RenderTarget 대응 G-Buffers 대응.

#ifndef __DEFINED_APPENDS_GBUFFER_TEXTURES_HLSL__
#define __DEFINED_APPENDS_GBUFFER_TEXTURES_HLSL__

#include "../SamplerStates/Appends_SamplerStates.hlsli"
#include "../Required/Appends_BufferSRVRegisters.hlsli"

float2 GetUV_F2(float2 quadUV)
{
    //RT0 : Texture UV Coords. (xy)
    return internal_GBuffer[0].Sample(fullScreenQuadSS, quadUV).xy;
}

float3 GetTex2DArrayUV_F3(float2 quadUV)
{
    //RT0 : Texture UV Coords w/ (xy) Mesh Material ID (z)
    return internal_GBuffer[0].Sample(fullScreenQuadSS, quadUV).xyz;
}

float GetMeshMatID(float2 quadUV)
{
    //RT0 : Mesh Material ID. (z)
    return internal_GBuffer[0].Sample(fullScreenQuadSS, quadUV).z;
}

float GetDepth_WDivide(float2 quadUV)
{
    //RT0 : W Divide Depth.
    return internal_GBuffer[0].Sample(fullScreenQuadSS, quadUV).w;
}

//더 이상 DSV를 직접 샘플링해서 사용하지는 않는다.

float3 GetNormal(float2 quadUV)
{
    //RT1 : World Space Normal. (xyz)
    return internal_GBuffer[1].Sample(fullScreenQuadSS, quadUV).xyz;
}

float3 GetPosition(float2 quadUV)
{
    //RT2 : World Space Position. (xyz)
    return internal_GBuffer[2].Sample(fullScreenQuadSS, quadUV).xyz;
}

float3 GetTangent(float2 quadUV)
{
    //RT3 : World Space Tangent (xyz)
    return internal_GBuffer[3].Sample(fullScreenQuadSS, quadUV).xyz;
}

float3 GetLightPixelPos(float2 quadUV)
{
    //LightPixelPos
    float val_x = internal_GBuffer[1].Sample(fullScreenQuadSS, quadUV).w;
    float val_y = internal_GBuffer[2].Sample(fullScreenQuadSS, quadUV).w;
    float val_z = internal_GBuffer[3].Sample(fullScreenQuadSS, quadUV).w;
   
    return float3(val_x, val_y, val_z);
}

float3 GetLightmapRGB(float2 quadUV)
{
    //RT4 : LightMap Sample Value (xyz) + Lightmapping이 활용되었는지(w). 음수 : NO, 양수 : YES.
    return internal_GBuffer[4].Sample(fullScreenQuadSS, quadUV).rgb;
}

bool IsUseLightmap(float2 quadUV, out bool isAlphaClipped)
{
    //0보다 크면 Lightmapping을 활용하는 것이다.
    int val = internal_GBuffer[4].Sample(fullScreenQuadSS, quadUV).w;
    isAlphaClipped = (val == 2);
    return val > 0;
}



#endif //__DEFINED_APPENDS_GBUFFER_TEXTURES_HLSL__