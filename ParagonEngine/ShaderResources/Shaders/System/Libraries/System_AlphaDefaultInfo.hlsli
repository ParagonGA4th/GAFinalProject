//�׷��� ���� ����Ʈ InputLayout�� ������ HLSL.

#ifndef __DEFINED_ALPHA_DEFAULT_INFO_HLSL__
#define __DEFINED_ALPHA_DEFAULT_INFO_HLSL__

//���� �÷��� ����Ѵ�.
cbuffer cbAlphaDefault : register(b1)
{
    float4x4 gCBuf_CameraViewProj; //VP
    float gCBuf_ObjectAlpha; //������Ʈ ���� ���� �ø���, 0-1.
    float3 gCBuf_LeftPadding; // �е�.
}


#endif //__DEFINED_ALPHA_DEFAULT_INFO_HLSL__