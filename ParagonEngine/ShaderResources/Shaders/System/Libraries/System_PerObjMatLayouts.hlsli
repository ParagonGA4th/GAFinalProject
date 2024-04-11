//그래픽 엔진 Per Object/Material의 InputLayout을 저장한 HLSL.

#ifndef __DEFINED_SYSTEM_PER_OBJMAT_LAYOUTS_HLSL__
#define __DEFINED_SYSTEM_PER_OBJMAT_LAYOUTS_HLSL__

struct VinPerObjMatStatic
{
    float3  vin1st_PosL         : POSITION;
    uint    vin1st_ObjID        : OBJECTID;
    uint    vin1st_MatID        : MATERIALID;
    float2  vin1st_Tex          : TEXCOORD;
    float   vin1st_MeshMatID    : MESH_MATID;
};

struct VinPerObjMatSkinned
{
    float3  vin1st_PosL         : POSITION;
    uint    vin1st_ObjID        : OBJECTID;
    uint    vin1st_MatID        : MATERIALID;
    float2  vin1st_Tex          : TEXCOORD;
    float   vin1st_MeshMatID    : MESH_MATID;
    uint    vin1st_NodeIndex    : NODE_INDEX;
    
    uint vin1st_BlendIndice0 : BLENDINDICES0;
    uint vin1st_BlendIndice1 : BLENDINDICES1;
    uint vin1st_BlendIndice2 : BLENDINDICES2;
    uint vin1st_BlendIndice3 : BLENDINDICES3;
    
    float vin1st_BlendWeight0 : BLENDWEIGHT0;
    float vin1st_BlendWeight1 : BLENDWEIGHT1;
    float vin1st_BlendWeight2 : BLENDWEIGHT2;
};

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
    // RT0 : DXGI_FORMAT_R32G32_FLOAT 기준.
    float2 pout_ObjMat : SV_Target0; // x : ObjID / y : MatID 

    // RT1 : DXGI_FORMAT_R32G32B32A32 기준. Albedo(RGB), AO(A)
    float4 pout_AlbedoAO : SV_Target1; 
    
    // RT2 : DXGI_FORMAT_R32G32B32A32 기준. Normal(RGB), Roughness(A)
    float4 pout_NormalRoughness : SV_Target2;
    
    // RT3 : DXGI_FORMAT_R32G32B32A32 기준. Specular (RGB), Metallic (A)
    float4 pout_SpecularMetallic : SV_Target3;
};

#endif //__DEFINED_SYSTEM_PER_OBJMAT_LAYOUTS_HLSL__