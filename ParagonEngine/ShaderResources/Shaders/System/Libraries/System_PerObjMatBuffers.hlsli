///Constant Buffer들을 셰이더들이 공유해서 사용할 수 있게 세팅하는 HLSL.
//Object ID / Material ID에 한정되게 활용하게 위해.

#ifndef __DEFINED_SYSTEM_PER_OBJMAT_BUFFERS_HLSL__
#define __DEFINED_SYSTEM_PER_OBJMAT_BUFFERS_HLSL__

//Defines
#define PG_MAX_OBJMAT_BONECOUNT 256
#define PG_MAX_OBJMAT_NODECOUNT 256

//Constant Buffers
cbuffer cbPerObjMatBase : register(b0)
{
    float4x4 gCBuf_World;
    float4x4 gCBuf_WorldViewProj;
};

cbuffer cbPerObjMatSkinnedNodes : register(b1)
{
    float4x4 gCBuf_Nodes[PG_MAX_OBJMAT_NODECOUNT];
};

cbuffer cbPerObjMatSkinnedBones : register(b2)
{
    float4x4 gCBuf_Bones[PG_MAX_OBJMAT_BONECOUNT];
};

#endif //__DEFINED_SYSTEM_PER_OBJMAT_BUFFERS_HLSL__
