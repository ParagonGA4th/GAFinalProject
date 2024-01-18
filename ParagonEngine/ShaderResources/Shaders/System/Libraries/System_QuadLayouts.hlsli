
#ifndef __DEFINED_SYSTEM_QUADLAYOUTS_HLSL__
#define __DEFINED_SYSTEM_QUADLAYOUTS_HLSL__

struct VinQuad
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
    float2 UV : TEXCOORD;
};

struct VOutQuad
{
    float4 PositionH : SV_POSITION;
    float2 UV : TEXCOORD;
};

struct POutQuad
{
    float4 Output : SV_Target;
};

//struct POutQuadWriteDepth
//{
//    float4 Output : SV_Target;
    
//};

#endif //__DEFINED_SYSTEM_QUADLAYOUTS_HLSL__