#include "../../Libraries/System_PerObjMatLayouts.hlsli"
#include "../../Libraries/System_PerObjMatBuffers.hlsli"

VOutPerObjMat main(VinPerObjMatStatic input)
{
    VOutPerObjMat output;
    
    // Position을 Local -> World 이동.
    output.vout1st_PosW = mul(gCBuf_World, float4(input.vin1st_PosL, 1.0f)).xyz;
	// 동차좌표계 내 Position 계산.
    output.vout1st_PosH = mul(gCBuf_WorldViewProj, float4(input.vin1st_PosL, 1.0f));
    
    //ObjectID, MaterialID 옮기기.
    output.vout1st_ObjID = input.vin1st_ObjID;
    output.vout1st_MatID = input.vin1st_MatID;
    
    return output;
}