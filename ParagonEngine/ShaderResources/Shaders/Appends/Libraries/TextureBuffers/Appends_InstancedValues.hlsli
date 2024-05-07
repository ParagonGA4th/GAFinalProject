#ifndef __DEFINED_APPENDS_INSTANCED_VALUES_HLSL__
#define __DEFINED_APPENDS_INSTANCED_VALUES_HLSL__

#include "../SamplerStates/Appends_SamplerStates.hlsli"

//Instancing ID가 같이 들어온다.

//ConstantBuffer로 SV_InstanceID로 라이트맵의 인덱스를 접근,
//이를 기반으로 다시 Texture2DArray 샘플링.

#define MAXIMUM_OBJECT_COUNT_PER_INSTANCING 1024

//유니티에서 라이트맵 구울 때 1024개 안쪽으로 무조건 세팅하게 하기.
//Scene이 바뀔 때만 올라간다.
//한번 넣어놓고 끝까지 유지, Scene이 바뀌기 전까지.
struct LightMapSet
{
    float2 scale;
    float2 offset;
    uint lightmapID;
    float3 padding;
};

cbuffer cbLightmapCollection : register(b5)
{
    LightMapSet gBuf_LightMapSet[MAXIMUM_OBJECT_COUNT_PER_INSTANCING];
};

Texture2DArray<float4> internal_LightmapArray : register(t2);

float4 GetLightmapData(float2 lightmapUV, uint instanceID)
{
    //UV Scaling.
    float2 tNewUV = lightmapUV;
    tNewUV.x *= gBuf_LightMapSet[instanceID].scale.x;
    tNewUV.y = (1 - tNewUV.y);
    tNewUV.y *= gBuf_LightMapSet[instanceID].scale.y;
    tNewUV.y = (1 - tNewUV.y);
    
    //UV Offset.
    tNewUV.x += (gBuf_LightMapSet[instanceID].offset.x);
    tNewUV.y += (-gBuf_LightMapSet[instanceID].offset.y);
 
    //실제로 값 샘플링할 준비.
    float3 tUV = float3(lightmapUV, (float) gBuf_LightMapSet[instanceID].lightmapID);
    return internal_LightmapArray.Sample(lightmapSS, tUV);
}

#endif //__DEFINED_APPENDS_INSTANCED_VALUES_HLSL__