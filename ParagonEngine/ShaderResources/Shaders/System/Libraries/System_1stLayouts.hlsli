//�׷��� ���� ����Ʈ InputLayout�� ������ HLSL.

#ifndef __DEFINED_SYSTEM_1STLAYOUTS_HLSL__
#define __DEFINED_SYSTEM_1STLAYOUTS_HLSL__

//�����ϰ� Lightmapping�� ����� ����. 
struct Vin1stPassInstanced_Layout
{
    //Vin1stStatic
    float3 vin1st_PosL : POSITION;
    float2 vin1st_Tex : TEXCOORD0;
    float vin1st_MeshMatID : MESH_MATID;
    float2 vin1st_LightmapUV : TEXCOORD1;
    
    //Vin2ndAll
    float3 vin1st_NormalL : NORMAL;
    float3 vin1st_TangentL : TANGENT;
    float3 vin1st_Color : COLOR;
    
    //Vin3rdInstanced 
    //�Ѳ����� ObjMat + Transform�� ���´�.
    // INSTANCING
    row_major float4x4 vin1st_Transform : TRANSFORM;
    
    uint vin1st_ObjID : OBJECTID;
    uint vin1st_MatID : MATERIALID;

    uint vin1st_InstanceID : SV_InstanceID; // ���ÿ� Lightmapping ID�� �� ���̴�.
};

struct Vin1stPassStatic_Layout
{
    //Vin1stStatic
    float3 vin1st_PosL         : POSITION;
    float2 vin1st_Tex          : TEXCOORD0;
    float  vin1st_MeshMatID    : MESH_MATID;
    float2 vin1st_LightmapUV   : TEXCOORD1;
    
    //Vin2ndAll
    float3  vin1st_NormalL      : NORMAL;
    float3  vin1st_TangentL     : TANGENT;
    float3  vin1st_Color        : COLOR;
};

struct Vin1stPassSkinned_Layout
{
    //Vin1stSkinned
    float3  vin1st_PosL         : POSITION;
    float2  vin1st_Tex          : TEXCOORD;
    float   vin1st_MeshMatID    : MESH_MATID;
    uint vin1st_BlendIndice0 : BLENDINDICES0;
    uint vin1st_BlendIndice1 : BLENDINDICES1;
    uint vin1st_BlendIndice2 : BLENDINDICES2;
    uint vin1st_BlendIndice3 : BLENDINDICES3;
    
    float vin1st_BlendWeight0 : BLENDWEIGHT0;
    float vin1st_BlendWeight1 : BLENDWEIGHT1;
    float vin1st_BlendWeight2 : BLENDWEIGHT2;
    float vin1st_BlendWeight3 : BLENDWEIGHT3;
    
    //Vin2ndAll
    float3 vin1st_NormalL : NORMAL;
    float3 vin1st_TangentL : TANGENT;
    float3 vin1st_Color : COLOR;
};

struct VOut1st
{
    //Vin1st�� �����.
    float4  vout1st_PosH        : SV_POSITION;
    float3  vout1st_PosW        : POSITION;
    float   vout1st_MeshMatID   : MESH_MATID;
    float2  vout1st_Tex         : TEXCOORD0;
    float2  vout1st_LightmapUV  : TEXCOORD1;
    
    //Vin2ndAll�� �����.
    float3  vout1st_NormalW     : NORMAL;
    float3  vout1st_TangentW    : TANGENT;
    //float3  vout1st_Color       : COLOR;
    //�� �̻� Color�� ������ �ʰ�, LightPosVec���� ��ü�Ǿ���. Divide�Ǿ ���´�.
    float3 vout1st_LightPixelPos : LIGHTPIXELPOS;
};

struct VOut1st_Instanced
{
    //Vin1st�� �����.
    float4 vout1st_PosH : SV_POSITION;
    float3 vout1st_PosW : POSITION;
    float vout1st_MeshMatID : MESH_MATID;
    float2 vout1st_Tex : TEXCOORD0;
    float2 vout1st_LightmapUV : TEXCOORD1;
    
    //Vin2ndAll�� �����.
    float3 vout1st_NormalW : NORMAL;
    float3 vout1st_TangentW : TANGENT;
    //float3 vout1st_Color : COLOR;
    //�� �̻� Color�� ������ �ʰ�, LightPosVec���� ��ü�Ǿ���. Divide�Ǿ ���´�.
    float3 vout1st_LightPixelPos : LIGHTPIXELPOS;

    //ObjMat + Instance ���.
    uint vout1st_ObjID : OBJECTID;
    uint vout1st_MatID : MATERIALID;
    
    //InstanceID (== Lightmapping Index�� �� ��)
    uint vout1st_InstanceID : INSTANCEID;
};

//�ν��Ͻ� ����� ������Ʈ���� �ѹ��� ó���� �� ���� ��.
struct POut1st_Total //RGBA
{
    //�̷��� ��������!
    float4 pout1st_RT0 : SV_Target0; //RT0 : Texture UV Coords (xy), Mesh Material ID (z), W Divide Depth (w)
    float4 pout1st_RT1 : SV_Target1; //RT1 : World Space Normal (xyz), World Space Tangent.x (w)
    float4 pout1st_RT2 : SV_Target2; //RT2 : World Space Position (xyz), World Space Tangent.y (w)
    float4 pout1st_RT3 : SV_Target3; //RT3 : Light Pixel Pos (xyz), World Space Tangent.z (w)
    float4 pout1st_RT4 : SV_Target4; //RT4 : LightMap Sample Value (xyz), IsUseLightmap(w)
    
    //3rd Pass ����.
    // RT0 : DXGI_FORMAT_R32G32B32A32_FLOAT ����.
    float4 pout_ObjMatAoR : SV_Target5; // x : ObjID / y : MatID / z : AO / w : Roughness

    // RT1 : DXGI_FORMAT_R32G32B32A32 ����. Albedo(RGB), Metallic (A)
    float4 pout_AlbedoMetallic : SV_Target6;
    
    // RT2 : DXGI_FORMAT_R32G32B32A32 ����. Normal(RGB), Alpha (A) (AlphaBlending ���� ��ü���� ��쿡�� ��ȿ�� ���̱⿡)
    float4 pout_NormalAlpha : SV_Target7;
    
    //DSV�ʹ� ���� ����.
};

#endif //__DEFINED_SYSTEM_1STLAYOUTS_HLSL__