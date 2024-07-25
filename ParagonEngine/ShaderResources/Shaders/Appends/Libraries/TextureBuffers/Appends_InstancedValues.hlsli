#ifndef __DEFINED_APPENDS_INSTANCED_VALUES_HLSL__
#define __DEFINED_APPENDS_INSTANCED_VALUES_HLSL__

#include "../SamplerStates/Appends_SamplerStates.hlsli"
#include "../SceneInfo/Appends_SceneInfoVSPS.hlsli"

//Instancing ID가 같이 들어온다.

//ConstantBuffer로 SV_InstanceID로 라이트맵의 인덱스를 접근,
//이를 기반으로 다시 Texture2DArray 샘플링.

#define MAXIMUM_OBJECT_COUNT_PER_INSTANCING 1024

//유니티에서 라이트맵 구울 때 1024개 안쪽으로 무조건 세팅하게 하기.
//Scene이 바뀔 때만 올라간다.
//한번 넣어놓고 끝까지 유지, Scene이 바뀌기 전까지.
struct SingleLightMapSet
{
    float2 scale;
    float2 offset;
    uint lightmapID;
    float3 padding;
};

struct SingleObjMatIdSet
{
    uint objID;
    uint matID;
};

//Scene 바뀔 때마다 업데이트.
cbuffer cbLightmapCollection : register(b6)
{
    SingleLightMapSet gBuf_LightMapSet[MAXIMUM_OBJECT_COUNT_PER_INSTANCING];
};

//Scene 바뀔 때마다 업데이트.
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
    float3 tUV = float3(tNewUV, (float) gBuf_LightMapSet[instanceID].lightmapID);
    float4 tLightedVal = internal_LightmapArray.Sample(lightmapSS, tUV);
    
    //테스트.
    //tLightedVal.xyz = pow(tLightedVal.xyz, 2.2f);
    
    float4 tRet = float4(tLightedVal.xyz, 1.0f);
    
    //if (gBuf_LightMapSet[instanceID].lightmapID == 0)
    //{
    //    tRet = float4(tLightedVal.x, 0, 0, 1);
    //}
    //else if (gBuf_LightMapSet[instanceID].lightmapID == 1)
    //{
    //    tRet = float4(0, tLightedVal.y, 0, 1);
    //}
    //else if (gBuf_LightMapSet[instanceID].lightmapID == 2)
    //{
    //    tRet = float4(0, 0, tLightedVal.z, 1);
    //}
    //else if (gBuf_LightMapSet[instanceID].lightmapID == 3)
    //{
    //    tRet = float4(tLightedVal.x, tLightedVal.y, 0, 1);
    //   
    //}
    //else if (gBuf_LightMapSet[instanceID].lightmapID == 4)
    //{
    //    tRet = float4(0, tLightedVal.y, tLightedVal.z, 1);
    //}
    //else
    //{
    //    tRet = float4(0, 0, 0, 1);
    //}
    return tRet;
}

#endif //__DEFINED_APPENDS_INSTANCED_VALUES_HLSL__