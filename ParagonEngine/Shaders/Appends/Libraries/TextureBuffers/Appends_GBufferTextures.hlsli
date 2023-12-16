///Quad에서 전달되는 기본적인 RenderTarget 대응 G-Buffers 대응.

#ifndef __DEFINED_APPENDS_GBUFFER_TEXTURES_HLSL__
#define __DEFINED_APPENDS_GBUFFER_TEXTURES_HLSL__

#include "../SamplerStates/Appends_SamplerStates.hlsli"

//정확한 사용처 : "Render Target DXGI Format"에 정리되어 있음.
Texture2D<float4> GBuffer[5] : register(t0);
Texture2D<float> DepthBuffer : register(t1);

float3 GetUV_F3(float2 quadUV)
{
    return GBuffer[0].Sample(fullScreenQuadSS, quadUV).xyz;
}

float2 GetUV_F2(float2 quadUV)
{
    return GBuffer[0].Sample(fullScreenQuadSS, quadUV).xy;
}

float3 GetTangent(float2 quadUV)
{
   float tanx = GBuffer[0].Sample(fullScreenQuadSS, quadUV).w;
   float tany = GBuffer[1].Sample(fullScreenQuadSS, quadUV).w;
   float tanz = GBuffer[2].Sample(fullScreenQuadSS, quadUV).w;
   
   return float3(tanx, tany, tanz);
}

float3 GetNormal(float2 quadUV)
{
    return GBuffer[1].Sample(fullScreenQuadSS, quadUV).xyz;
}

float3 GetPosition(float2 quadUV)
{
    return GBuffer[2].Sample(fullScreenQuadSS, quadUV).xyz;
}

float4 GetVertexColor(float2 quadUV)
{
    return GBuffer[3].Sample(fullScreenQuadSS, quadUV).xyzw;
}

float2 GetLightmapUV(float2 quadUV)
{
    return GBuffer[4].Sample(fullScreenQuadSS, quadUV).xy;
}

float GetAlpha(float2 quadUV)
{
    return GBuffer[4].Sample(fullScreenQuadSS, quadUV).z;
}

uint GetObjectID(float2 quadUV)
{
    return asuint(GBuffer[4].Sample(fullScreenQuadSS, quadUV).w);
}

//Depth: 별도로 관리됨.
float GetDepth(float2 quadUV)
{
    return DepthBuffer.Sample(fullScreenQuadSS, quadUV);
}

#endif //__DEFINED_APPENDS_GBUFFER_TEXTURES_HLSL__