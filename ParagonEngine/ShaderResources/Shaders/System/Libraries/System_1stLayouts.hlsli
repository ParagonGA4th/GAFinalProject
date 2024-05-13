//그래픽 엔진 디폴트 InputLayout을 저장한 HLSL.

#ifndef __DEFINED_SYSTEM_1STLAYOUTS_HLSL__
#define __DEFINED_SYSTEM_1STLAYOUTS_HLSL__

//유일하게 Lightmapping이 적용될 예정. 
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
    //한꺼번에 ObjMat + Transform이 들어온다.
    uint vin1st_ObjID : OBJECTID;
    uint vin1st_MatID : MATERIALID;
    // INSTANCING
    row_major float4x4 vin1st_Transform : TRANSFORM;  // ColumnMajor로 들어온다.
    uint vin1st_InstanceID : SV_InstanceID; // 동시에 Lightmapping ID가 될 것이다.
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
    
    //Vin3rdStaticSkinned
    uint vin1st_ObjID : OBJECTID;
    uint vin1st_MatID : MATERIALID;
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
    
    uint vin1st_NodeIndex       : NODE_INDEX;
    
    //Vin2ndAll
    float3 vin1st_NormalL : NORMAL;
    float3 vin1st_TangentL : TANGENT;
    float3 vin1st_Color : COLOR;
    
    //Vin3rdStaticSkinned
    uint vin1st_ObjID : OBJECTID;
    uint vin1st_MatID : MATERIALID;
};

struct VOut1st
{
    //Vin1st의 결과물.
    float4  vout1st_PosH        : SV_POSITION;
    float3  vout1st_PosW        : POSITION;
    float   vout1st_MeshMatID   : MESH_MATID;
    float2  vout1st_Tex         : TEXCOORD0;
    float2  vout1st_LightmapUV  : TEXCOORD1;
    
    //Vin2ndAll의 결과물.
    float3  vout1st_NormalW     : NORMAL;
    float3  vout1st_TangentW    : TANGENT;
    float3  vout1st_Color       : COLOR;
    
    //ObjMat + Instance 기록.
    uint vout1st_ObjID : OBJECTID;
    uint vout1st_MatID : MATERIALID;
};

struct VOut1st_Instanced
{
    //Vin1st의 결과물.
    float4 vout1st_PosH : SV_POSITION;
    float3 vout1st_PosW : POSITION;
    float vout1st_MeshMatID : MESH_MATID;
    float2 vout1st_Tex : TEXCOORD0;
    float2 vout1st_LightmapUV : TEXCOORD1;
    
    //Vin2ndAll의 결과물.
    float3 vout1st_NormalW : NORMAL;
    float3 vout1st_TangentW : TANGENT;
    float3 vout1st_Color : COLOR;

    //ObjMat + Instance 기록.
    uint vout1st_ObjID : OBJECTID;
    uint vout1st_MatID : MATERIALID;
    
    //InstanceID (== Lightmapping Index가 될 것)
    uint vout1st_InstanceID : INSTANCEID;
};

//인스턴싱 적용된 오브젝트들은 한번에 처리할 수 있을 것.
struct POut1st_Total //RGBA
{
    //이렇게 유지하자!
    float4 pout1st_RT0 : SV_Target0; //RT0 : Texture UV Coords (xy), Mesh Material ID (z), W Divide Depth (w)
    float4 pout1st_RT1 : SV_Target1; //RT1 : World Space Normal (xyz), World Space Tangent.x (w)
    float4 pout1st_RT2 : SV_Target2; //RT2 : World Space Position (xyz), World Space Tangent.y (w)
    float4 pout1st_RT3 : SV_Target3; //RT3 : 3D Model Color (xyz), World Space Tangent.z (w)
    float4 pout1st_RT4 : SV_Target4; //RT4 : LightMap Sample Value (xy), BLANK (zw)
    
    //3rd Pass 통합.
    // RT0 : DXGI_FORMAT_R32G32B32A32_FLOAT 기준.
    float4 pout_ObjMatAoR : SV_Target5; // x : ObjID / y : MatID / z : AO / w : Roughness

    // RT1 : DXGI_FORMAT_R32G32B32A32 기준. Albedo(RGB), Metallic (A)
    float4 pout_AlbedoMetallic : SV_Target6;
    
    // RT2 : DXGI_FORMAT_R32G32B32A32 기준. Normal(RGB), Alpha (A) (AlphaBlending 쓰는 객체들의 경우에는 유효할 것이기에)
    float4 pout_NormalAlpha : SV_Target7;
    
    //DSV와는 관계 없다.
};

#endif //__DEFINED_SYSTEM_1STLAYOUTS_HLSL__