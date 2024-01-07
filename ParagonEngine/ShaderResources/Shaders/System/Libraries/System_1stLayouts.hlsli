//그래픽 엔진 디폴트 InputLayout을 저장한 HLSL.

#ifndef __DEFINED_SYSTEM_1STLAYOUTS_HLSL__
#define __DEFINED_SYSTEM_1STLAYOUTS_HLSL__

struct Vin1stStatic
{
    float3  vin1st_PosL         : POSITION;
    float   vin1st_Alpha        : ALPHA; 
    float3  vin1st_NormalL      : NORMAL;
    float3  vin1st_TangentL     : TANGENT;
    float3  vin1st_Color        : COLOR;
    float3  vin1st_Tex          : TEXCOORD0;
    float2  vin1st_UVSet2       : TEXCOORD1;
    float2  vin1st_LightmapUV   : TEXCOORD2;
};

struct Vin1stSkinned
{
    float3  vin1st_PosL         : POSITION;
    float   vin1st_Alpha        : ALPHA; 
    float3  vin1st_NormalL      : NORMAL;
    float3  vin1st_TangentL     : TANGENT;
    float3  vin1st_Color        : COLOR;
    float3  vin1st_Tex          : TEXCOORD0;
    float2  vin1st_UVSet2       : TEXCOORD1;
    float2  vin1st_LightmapUV   : TEXCOORD2;
    
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
    float4  vout1st_PosH        : SV_POSITION;
    float3  vout1st_PosW        : POSITION;
    float   vout1st_Alpha       : ALPHA;
    float3  vout1st_NormalW     : NORMAL;
    float3  vout1st_TangentW    : TANGENT;
    float3  vout1st_Color       : COLOR;
    float3  vout1st_Tex         : TEXCOORD0;
    float2  vout1st_UVSet2      : TEXCOORD1;
    float2  vout1st_LightmapUV  : TEXCOORD2;
};

struct POut1st //RGBA
{
    float4 pout1st_RT0 : SV_Target0; //RT0 : Texture UV Coords (xyz), Alpha (w)
    float4 pout1st_RT1 : SV_Target1; //RT1 : World Space Normal (xyz), World Space Tangent.x (w)
    float4 pout1st_RT2 : SV_Target2; //RT2 : World Space Position (xyz), World Space Tangent.y (w)
    float4 pout1st_RT3 : SV_Target3; //RT3 : 3D Model Color (xyz), World Space Tangent.z (w)
    float4 pout1st_RT4 : SV_Target4; //RT4 : UVSet2 (xy), LightMap Texture UV Coords (zw)
    //Depth Stencil 딴에서 마지막 GBuffer가 만들어진다.
};

#endif //__DEFINED_SYSTEM_1STLAYOUTS_HLSL__