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
    uint vin1st_MatID : MATERIALID;
};

struct Vin1stSkinned
{
    float3 vin1st_PosL : POSITION;
    float3 vin1st_NormalL : NORMAL;
    float3 vin1st_TangentL : TANGENT;
    float4 vin1st_Color : COLOR;
    float3 vin1st_Tex : TEXCOORD;
    uint vin1st_MatID : MATERIALID;
    
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
    uint vout1st_MatID : MATERIALID;
};

struct POut1st //RGBA
{
    float4 pout1st_RT0 : SV_Target0; // Texture UV Coords (RGB),  World Space Tangent.x (A)
    float4 pout1st_RT1 : SV_Target1; // World Space Normal (RGB), World Space Tangent.y (A)
    float4 pout1st_RT2 : SV_Target2; // World Space Position (RGB), World Space Tangent.z (A)
    float4 pout1st_RT3 : SV_Target3; // 3D Model Color (RGB)
    float4 pout1st_RT4 : SV_Target4; // Material ID ((uint)R), Screen Space Velocity (GB), Alpha (A)
    float4 pout1st_RT5 : SV_Target5; // Temp Diffuse Texture;
	float4 pout1st_RT6 : SV_Target6; // Phong Lighting Results
    //float pout1st_Depth : SV_Depth; // Depth
};

struct VinLighting
{
	float3 PositionW : POSITION;
	float3 Nomral : NORMAL;
	float2 UV : TEXCOORD;
};

struct VOutLighting
{
	float4 PositionH : SV_POSITION;
	float2 UV : TEXCOORD;
};

struct POutLighting
{
	float4 Output : SV_Target;
};

struct VInFinal
{
	float3 Position : POSITION;
	float3 Nomral : NORMAL;
	float2 UV : TEXCOORD;
};

struct VOutFinal
{
	float4 PositionH : SV_POSITION;
	float2 UV : TEXCOORD;
};

struct POutFinal
{
	float4 Output : SV_Target;
};

#endif //__DEFINED_DEFAULT_LAYOUTS_HLSL__