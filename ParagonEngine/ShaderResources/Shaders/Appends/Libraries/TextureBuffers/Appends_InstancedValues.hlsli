#ifndef __DEFINED_APPENDS_INSTANCED_VALUES_HLSL__
#define __DEFINED_APPENDS_INSTANCED_VALUES_HLSL__

#include "../SamplerStates/Appends_SamplerStates.hlsli"
#include "../SceneInfo/Appends_SceneInfoVSPS.hlsli"

//Instancing ID�� ���� ���´�.

//ConstantBuffer�� SV_InstanceID�� ����Ʈ���� �ε����� ����,
//�̸� ������� �ٽ� Texture2DArray ���ø�.

#define MAXIMUM_OBJECT_COUNT_PER_INSTANCING 1024

//����Ƽ���� ����Ʈ�� ���� �� 1024�� �������� ������ �����ϰ� �ϱ�.
//Scene�� �ٲ� ���� �ö󰣴�.
//�ѹ� �־���� ������ ����, Scene�� �ٲ�� ������.
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

//Scene �ٲ� ������ ������Ʈ.
cbuffer cbLightmapCollection : register(b6)
{
    SingleLightMapSet gBuf_LightMapSet[MAXIMUM_OBJECT_COUNT_PER_INSTANCING];
};

//Scene �ٲ� ������ ������Ʈ.
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
 
    //������ �� ���ø��� �غ�.
    float3 tUV = float3(tNewUV, (float) gBuf_LightMapSet[instanceID].lightmapID);
    float4 tLightedVal = internal_LightmapArray.Sample(lightmapSS, tUV);
    
    //�׽�Ʈ.
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