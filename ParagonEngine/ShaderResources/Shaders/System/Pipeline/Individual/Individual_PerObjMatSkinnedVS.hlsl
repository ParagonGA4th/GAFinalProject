#include "../../Libraries/System_PerObjMatLayouts.hlsli"
#include "../../Libraries/System_PerObjMatBuffers.hlsli"

VOutPerObjMat main(VinPerObjMatSkinned input)
{
    VOutPerObjMat output;
    
    float LastWeight = saturate(1.0f - input.vin1st_BlendWeight0 - input.vin1st_BlendWeight1 - input.vin1st_BlendWeight2);
    matrix finalOffsetMatrix = mul(mul(gCBuf_Bones[input.vin1st_BlendIndice0], gCBuf_Nodes[input.vin1st_BlendIndice0]), input.vin1st_BlendWeight0) +
                               mul(mul(gCBuf_Bones[input.vin1st_BlendIndice1], gCBuf_Nodes[input.vin1st_BlendIndice1]), input.vin1st_BlendWeight1) +
                               mul(mul(gCBuf_Bones[input.vin1st_BlendIndice2], gCBuf_Nodes[input.vin1st_BlendIndice2]), input.vin1st_BlendWeight2) +
                               mul(mul(gCBuf_Bones[input.vin1st_BlendIndice3], gCBuf_Nodes[input.vin1st_BlendIndice3]), LastWeight);
    
    float3 skinnedPosL = mul(float4(input.vin1st_PosL, 1.0f), finalOffsetMatrix);
	
	//NodeIndex를 쓰지 않는다..?? 일단 보류.
	
	// Position을 Local -> World 이동.
    output.vout1st_PosW = mul(float4(skinnedPosL, 1.0f), gCBuf_World).xyz;
	
    // 동차좌표계 내 Position 계산.
    output.vout1st_PosH = mul(float4(skinnedPosL, 1.0f), gCBuf_WorldViewProj);
	
	// Position을 Local -> World 이동.
    output.vout1st_PosW = mul(float4(skinnedPosL, 1.0f), gCBuf_World).xyz;
    
    //ObjectID, MaterialID 옮기기.
    output.vout1st_ObjID = input.vin1st_ObjID;
    output.vout1st_MatID = input.vin1st_MatID;
    
    output.vout1st_Tex          = input.vin1st_Tex;
    output.vout1st_MeshMatID = input.vin1st_MeshMatID;
    
    return output;
}