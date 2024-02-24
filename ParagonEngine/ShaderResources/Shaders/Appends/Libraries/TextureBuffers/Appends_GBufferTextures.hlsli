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

float GetMeshMatID(float2 quadUV)
{
    //RT0 : Mesh Material ID. (z)
    return internal_GBuffer[0].Sample(fullScreenQuadSS, quadUV).z;
}

float3 GetTex2DArrayUV_F3(float2 quadUV)
{
    //RT0 : Texture UV Coords w/ (xy) Mesh Material ID (z)
    return internal_GBuffer[0].Sample(fullScreenQuadSS, quadUV).xyz;
}

float GetAlpha(float2 quadUV)
{
    //RT0 : Alpha (w)
    return internal_GBuffer[0].Sample(fullScreenQuadSS, quadUV).w;
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

float3 GetVertexColor(float2 quadUV)
{
    //RT3 : 3D Model Color. (For Blending) (xyz)
    return internal_GBuffer[3].Sample(fullScreenQuadSS, quadUV).xyz;
}

float3 GetTangent(float2 quadUV)
{
   //Tangent Collection
    float tanx = internal_GBuffer[1].Sample(fullScreenQuadSS, quadUV).w;
    float tany = internal_GBuffer[2].Sample(fullScreenQuadSS, quadUV).w;
    float tanz = internal_GBuffer[3].Sample(fullScreenQuadSS, quadUV).w;
   
    return float3(tanx, tany, tanz);
}

float2 GetUVSet2(float2 quadUV)
{
    return internal_GBuffer[4].Sample(fullScreenQuadSS, quadUV).xy;
}

float2 GetLightmapUV(float2 quadUV)
{
    //RT4 : LightMap Texture UV Coords (zw)
    return internal_GBuffer[4].Sample(fullScreenQuadSS, quadUV).zw;
}

//Depth: 별도로 관리됨.
float GetDepth(float2 quadUV)
{
    return internal_DepthBuffer.Sample(fullScreenQuadSS, quadUV);
}

//float GetLinearDepth(float2 quadUV)
//{
//    float d = internal_DepthBuffer.Sample(fullScreenQuadSS, quadUV);
//    //float depth = internal_DepthBuffer.Sample(fullScreenQuadSS, quadUV);
//    
//   //// NDC로 다시 변환.
//   //float z = depth * 2.0 - 1.0; 
//   //
//   // //현재로서는 Near : 0.1 / Far : 300. 나중에 연동되면 좋을 듯.
//   //float near = 0.1f;
//   //float far = 300.0f;
//   //
//   //return (2.0 * near * far) / (far + near - z * (far - near));
//    float farNearRatio = 300.0f / 0.1f;
//   return d / (farNearRatio * (1.0f - d) + d);
//}  

#endif //__DEFINED_APPENDS_GBUFFER_TEXTURES_HLSL__