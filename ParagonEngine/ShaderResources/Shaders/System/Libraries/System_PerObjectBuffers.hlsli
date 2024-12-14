///Constant Buffer���� ���̴����� �����ؼ� ����� �� �ְ� �����ϴ� HLSL.

#ifndef __DEFINED_SYSTEM_PER_OBJECT_BUFFERS_HLSL__
#define __DEFINED_SYSTEM_PER_OBJECT_BUFFERS_HLSL__

#include "System_SwitchingViewProj.hlsli"

//Defines
#define PG_MAX_BONECOUNT 256
#define PG_MAX_NODECOUNT 256

//Constant Buffers
cbuffer cbPerObjectBase : register(b0)
{
    //��� �������� Column Major�� ���� ��.
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
