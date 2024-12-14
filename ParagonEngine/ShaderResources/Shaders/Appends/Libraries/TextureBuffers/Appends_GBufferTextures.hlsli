///Quad���� ���޵Ǵ� �⺻���� RenderTarget ���� G-Buffers ����.

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

//�� �̻� DSV�� ���� ���ø��ؼ� ��������� �ʴ´�.

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
    //RT4 : LightMap Sample Value (xyz) + Lightmapping�� Ȱ��Ǿ�����(w). ���� : NO, ��� : YES.
    return internal_GBuffer[4].Sample(fullScreenQuadSS, quadUV).rgb;
}

bool IsUseLightmap(float2 quadUV, out bool isAlphaClipped)
{
    //0���� ũ�� Lightmapping�� Ȱ���ϴ� ���̴�.
    int val = internal_GBuffer[4].Sample(fullScreenQuadSS, quadUV).w;
    isAlphaClipped = (val == 2);
    return val > 0;
}



#endif //__DEFINED_APPENDS_GBUFFER_TEXTURES_HLSL__