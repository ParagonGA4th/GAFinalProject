#include "../../Libraries/System_PerObjMatLayouts.hlsli"
#include "../../Libraries/System_PerObjMatBuffers.hlsli"

VOutPerObjMat main(VinPerObjMatSkinned input)
{
    VOutPerObjMat output;
    
    float _weights[4] = { 0.f, 0.f, 0.f, 0.f };
    _weights[0] = input.vin1st_BlendWeight0;
    _weights[1] = input.vin1st_BlendWeight1;
    _weights[2] = input.vin1st_BlendWeight2;
    _weights[3] = 1 - (_weights[0] + _weights[1] + _weights[2]);

    float3 skinnedPosL = float3(0.f, 0.f, 0.f);
    
    skinnedPosL += _weights[0] * mul(float4(input.vin1st_PosL, 1.f), gCBuf_Bones[input.vin1st_BlendIndice0]).xyz;
    skinnedPosL += _weights[1] * mul(float4(input.vin1st_PosL, 1.f), gCBuf_Bones[input.vin1st_BlendIndice1]).xyz;
    skinnedPosL += _weights[2] * mul(float4(input.vin1st_PosL, 1.f), gCBuf_Bones[input.vin1st_BlendIndice2]).xyz;
    skinnedPosL += _weights[3] * mul(float4(input.vin1st_PosL, 1.f), gCBuf_Bones[input.vin1st_BlendIndice3]).xyz;	
	
	// 동차좌표계 내 Position 계산.
    output.vout1st_PosH = mul(float4(skinnedPosL, 1.0f), gCBuf_WorldViewProj);
	
	// Position을 Local -> World 이동.
    output.vout1st_PosW = mul(float4(skinnedPosL, 1.0f), gCBuf_World).xyz;
    
    //ObjectID, MaterialID 옮기기.
    output.vout1st_ObjID = input.vin1st_ObjID;
    output.vout1st_MatID = input.vin1st_MatID;
    
    return output;
}