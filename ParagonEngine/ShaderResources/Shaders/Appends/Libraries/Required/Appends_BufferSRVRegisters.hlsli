#ifndef __DEFINED_APPENDS_BUFFER_REGISTER_SRV_HLSL__
#define __DEFINED_APPENDS_BUFFER_REGISTER_SRV_HLSL__

//정확한 사용처 : "Render Target DXGI Format"에 정리되어 있음.
Texture2D<float4> internal_GBuffer[5] : register(t12); // RT0-RT4

//모두 RGB / A. Quad-Based.
Texture2D<float4> internal_t2_ObjMatAoR : register(t17); //RT5
Texture2D<float4> internal_t2_AlbedoMetallic : register(t18); //RT6
Texture2D<float4> internal_t2_NormalAlpha : register(t19); // RT7

//직접 DSV를 샘플링하지는 않을 것이다.

#endif //__DEFINED_APPENDS_BUFFER_REGISTER_SRV_HLSL__
