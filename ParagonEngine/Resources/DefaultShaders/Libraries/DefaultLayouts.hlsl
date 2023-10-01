//그래픽 엔진 디폴트 InputLayout을 저장한 HLSL.

#ifndef __DEFINED_DEFAULT_LAYOUTS_HLSL__
#define __DEFINED_DEFAULT_LAYOUTS_HLSL__

struct Vin1stStatic
{
    float3 vin1st_PosL : POSITION;
    float3 vin1st_NormalL : NORMAL;
    float3 vin1st_TangentL : TANGENT;
    float4 vin1st_Color : COLOR;
    float3 vin1st_Tex : TEXCOORD;
    //uint vin1st_MatID : MATERIALID;
};

struct Vin1stSkinned
{
    float3 vin1st_PosL : POSITION;
    float3 vin1st_NormalL : NORMAL;
    float3 vin1st_TangentL : TANGENT;
    float4 vin1st_Color : COLOR;
    float3 vin1st_Tex : TEXCOORD;
    //uint vin1st_MatID : MATERIALID;
    
    uint vin1st_BlendIndice0 : BLENDINDICES0;
    uint vin1st_BlendIndice1 : BLENDINDICES1;
    uint vin1st_BlendIndice2 : BLENDINDICES2;
    uint vin1st_BlendIndice3 : BLENDINDICES3;
    
    float vin1st_BlendWeight0 : BLENDWEIGHT0;
    float vin1st_BlendWeight1 : BLENDWEIGHT1;
    float vin1st_BlendWeight2 : BLENDWEIGHT2;
};

struct VOut1st
{
    float4 vout1st_PosH : SV_POSITION;
    float3 vout1st_PosW : POSITION;
    float3 vout1st_NormalW : NORMAL;
    float3 vout1st_TangentW : TANGENT;
    float4 vout1st_Color : COLOR;
    float3 vout1st_Tex : TEXCOORD;
    //uint vout1st_MatID : MATERIALID;
};

struct POut1st
{
    float4 pout1st_RT1 : SV_Target0;
    float4 pout1st_RT2 : SV_Target0;
    float4 pout1st_RT3 : SV_Target0;
    float4 pout1st_RT4 : SV_Target0;
    float4 pout1st_RT1 : SV_Target0;
    
    //TYPELESS는 어떻게 넣지?
};

#endif //__DEFINED_DEFAULT_LAYOUTS_HLSL__