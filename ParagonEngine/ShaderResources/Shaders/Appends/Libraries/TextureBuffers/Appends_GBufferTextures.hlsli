///Quad에서 전달되는 기본적인 RenderTarget 대응 G-Buffers 대응.

#ifndef __DEFINED_APPENDS_GBUFFER_TEXTURES_HLSL__
#define __DEFINED_APPENDS_GBUFFER_TEXTURES_HLSL__

#include "../SamplerStates/Appends_SamplerStates.hlsli"

//정확한 사용처 : "Render Target DXGI Format"에 정리되어 있음.
Texture2D<float4> internal_GBuffer[5] : register(t15);

//DXGI_FORMAT_R32_TYPELESS로 만들어질 예정 -> 이 중, D24로 해석될 것이다.
Texture2D<float> internal_DepthBuffer : register(t20);

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

float2 GetDepth_WDivide(float2 quadUV)
{
    //RT0 : W Divide Depth.
    return internal_GBuffer[0].Sample(fullScreenQuadSS, quadUV).w;
}

//DSV Depth: 별도로 관리됨.
float GetDepth_DSV(float2 quadUV)
{
    return internal_DepthBuffer.Sample(fullScreenQuadSS, quadUV);
}

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

float3 GetVertexColor(float2 quadUV)
{
    //VertexBlending Color.
    float color_x = internal_GBuffer[1].Sample(fullScreenQuadSS, quadUV).w;
    float color_y = internal_GBuffer[2].Sample(fullScreenQuadSS, quadUV).w;
    float color_z = internal_GBuffer[3].Sample(fullScreenQuadSS, quadUV).w;
   
    return float3(color_x, color_y, color_z);
}

float3 GetLightmapRGB(float2 quadUV)
{
    //RT4 : LightMap Sample Value (xyz) + Lightmapping이 활용되었는지(w). 음수 : NO, 양수 : YES.
    return internal_GBuffer[4].Sample(fullScreenQuadSS, quadUV).rgb;
}

bool IsUseLightmap(float2 quadUV)
{
    //0보다 크면 Lightmapping을 활용하는 것이다.
    return (internal_GBuffer[4].Sample(fullScreenQuadSS, quadUV).w > 0);
}

#endif //__DEFINED_APPENDS_GBUFFER_TEXTURES_HLSL__