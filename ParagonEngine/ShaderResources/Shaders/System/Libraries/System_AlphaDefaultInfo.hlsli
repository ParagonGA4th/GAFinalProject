//그래픽 엔진 디폴트 InputLayout을 저장한 HLSL.

#ifndef __DEFINED_ALPHA_DEFAULT_INFO_HLSL__
#define __DEFINED_ALPHA_DEFAULT_INFO_HLSL__

//값을 올려서 사용한다.
cbuffer cbAlphaDefault : register(b1)
{
    float4x4 gCBuf_CameraViewProj; //VP
    float gCBuf_ObjectAlpha; //오브젝트 단위 알파 올리기, 0-1.
    float3 gCBuf_LeftPadding; // 패딩.
}


#endif //__DEFINED_ALPHA_DEFAULT_INFO_HLSL__