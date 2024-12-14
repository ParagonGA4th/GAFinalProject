#ifndef __DEFINED_APPENDS_BUFFER_REGISTER_SRV_HLSL__
#define __DEFINED_APPENDS_BUFFER_REGISTER_SRV_HLSL__

//��Ȯ�� ���ó : "Render Target DXGI Format"�� �����Ǿ� ����.
Texture2D<float4> internal_GBuffer[5] : register(t12); // RT0-RT4

//��� RGB / A. Quad-Based.
Texture2D<float4> internal_t2_ObjMatAoR : register(t17); //RT5
Texture2D<float4> internal_t2_AlbedoMetallic : register(t18); //RT6
Texture2D<float4> internal_t2_NormalAlpha : register(t19); // RT7

//���� DSV�� ���ø������� ���� ���̴�.

#endif //__DEFINED_APPENDS_BUFFER_REGISTER_SRV_HLSL__
