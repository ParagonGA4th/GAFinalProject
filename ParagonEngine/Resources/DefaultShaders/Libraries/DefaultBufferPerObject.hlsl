///Constant Buffer들을 셰이더들이 공유해서 사용할 수 있게 세팅하는 HLSL.

#ifndef __DEFINED_DEFAULT_BUFFER_PER_OBJECT_HLSL__
#define __DEFINED_DEFAULT_BUFFER_PER_OBJECT_HLSL__

//Defines
#define PG_MAX_BONECOUNT 100

//Constant Buffers
cbuffer cbPerObjectBase : register(b0)
{
    float4x4 gCBuf_World;
    float4x4 gCBuf_WorldInvTranspose;
    float4x4 gCBuf_WorldViewProj;
    float3 gCBuf_CameraPositionW;
    // gCBuf_Materials[10] //후에 Material이 들어가면 추가되어야.
};

cbuffer cbPerObjectSkinned : register(b1)
{
    float4x4 gCBuf_Bones[PG_MAX_BONECOUNT];
};

#endif //__DEFINED_DEFAULT_BUFFER_PER_OBJECT_HLSL__
