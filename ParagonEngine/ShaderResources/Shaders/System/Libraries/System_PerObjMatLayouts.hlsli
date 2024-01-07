//그래픽 엔진 Per Object/Material의 InputLayout을 저장한 HLSL.

#ifndef __DEFINED_SYSTEM_PER_OBJMAT_LAYOUTS_HLSL__
#define __DEFINED_SYSTEM_PER_OBJMAT_LAYOUTS_HLSL__

struct VinPerObjMatStatic
{
    float3  vin1st_PosL         : POSITION;
    uint    vin1st_ObjID        : OBJECTID;
    uint    vin1st_MatID        : MATERIALID;
};

struct VinPerObjMatSkinned
{
    float3 vin1st_PosL  : POSITION;
    uint vin1st_ObjID   : OBJECTID;
    uint vin1st_MatID   : MATERIALID;
    
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
};

struct POutPerObjMat //RG
{
    // DXGI_FORMAT_R32G32_UINT 기준.
    uint2 pout_ObjMat : SV_Target; // x : ObjID / y : MatID 
};

#endif //__DEFINED_SYSTEM_PER_OBJMAT_LAYOUTS_HLSL__