//그래픽 엔진 Per Object/Material의 InputLayout을 저장한 HLSL.

#ifndef __DEFINED_SYSTEM_PER_OBJMAT_LAYOUTS_HLSL__
#define __DEFINED_SYSTEM_PER_OBJMAT_LAYOUTS_HLSL__

struct VinPerThirdPassStatic
{
    //Vin1stStatic.
    float3  vin1st_PosL         : POSITION;
    float2  vin1st_Tex          : TEXCOORD0;
    float   vin1st_MeshMatID    : MESH_MATID;
    float2  vin1st_LightmapUV   : TEXCOORD1;
    
    //Vin3rdStaticSkinned
    uint    vin1st_ObjID        : OBJECTID;
    uint    vin1st_MatID        : MATERIALID;
};

struct VinPerThirdPassSkinned
{
    //Vin1stSkinned.
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
    uint vin1st_NodeIndex : NODE_INDEX;
    
    //Vin3rdStaticSkinned
    uint    vin1st_ObjID        : OBJECTID;
    uint    vin1st_MatID        : MATERIALID;
};

//얘 같은 경우는 비-인스턴싱 적용 한정.
struct VOutPerObjMat
{
    float4  vout1st_PosH        : SV_POSITION;
    float3  vout1st_PosW        : POSITION;
    uint    vout1st_ObjID       : OBJECTID;
    uint    vout1st_MatID       : MATERIALID;
    float   vout1st_MeshMatID   : MESH_MATID;
    float2  vout1st_Tex         : TEXCOORD;
};

struct POutPerObjMat //RG
{
    // RT0 : DXGI_FORMAT_R32G32B32A32_FLOAT 기준.
    float4 pout_ObjMatAoR : SV_Target0; // x : ObjID / y : MatID / z : AO(A) / w : Roughness(A)

    // RT1 : DXGI_FORMAT_R32G32B32A32 기준. Albedo(RGB), Metallic (A)
    float4 pout_AlbedoMetallic : SV_Target1; 
    
    // RT2 : DXGI_FORMAT_R32G32B32A32 기준. Normal(RGB), Alpha (A) (AlphaBlending 쓰는 객체들의 경우에는 유효할 것이기에)
    float4 pout_NormalAlpha : SV_Target2;
};

#endif //__DEFINED_SYSTEM_PER_OBJMAT_LAYOUTS_HLSL__