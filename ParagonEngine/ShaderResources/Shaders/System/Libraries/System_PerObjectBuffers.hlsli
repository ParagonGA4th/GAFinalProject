///Constant Buffer들을 셰이더들이 공유해서 사용할 수 있게 세팅하는 HLSL.

#ifndef __DEFINED_SYSTEM_PER_OBJECT_BUFFERS_HLSL__
#define __DEFINED_SYSTEM_PER_OBJECT_BUFFERS_HLSL__

#include "System_SwitchingViewProj.hlsli"

//Defines
#define PG_MAX_BONECOUNT 256
#define PG_MAX_NODECOUNT 256

//Constant Buffers
cbuffer cbPerObjectBase : register(b0)
{
    //얘는 내적으로 Column Major로 쓰일 것.
    float4x4 gCBuf_World;
    float4x4 gCBuf_WorldInvTranspose;
    uint gCBuf_ObjID;
    uint gCBuf_MatID;
};

cbuffer cbPerObjectSkinnedNodes : register(b2)
{
    float4x4 gCBuf_Nodes[PG_MAX_NODECOUNT];
};

cbuffer cbPerObjectSkinnedBones : register(b3)
{
    float4x4 gCBuf_Bones[PG_MAX_BONECOUNT];
};

#endif //__DEFINED_SYSTEM_PER_OBJECT_BUFFERS_HLSL__
