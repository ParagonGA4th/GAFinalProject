///그래픽엔진 자체의 1st Vertex Shader (Static)

#include "../Libraries/DefaultBufferPerObject.hlsli"
#include "../Libraries/DefaultLayouts.hlsli"

VOut1st VS_MAIN(Vin1stStatic input)
{
	VOut1st output;
	
	// Position을 Local -> World 이동.
	output.vout1st_PosW = mul(gCBuf_World, float4(input.vin1st_PosL, 1.0f)).xyz;
	
	// Normal을 Local -> World 이동.
	output.vout1st_NormalW = normalize(mul((float3x3)gCBuf_WorldInvTranspose, input.vin1st_NormalL));
	
	// Tangent를 Local -> World 이동.
	output.vout1st_TangentW = mul((float3x3)gCBuf_World, input.vin1st_TangentL);
	
	// 동차좌표계 내 Position 계산.
	output.vout1st_PosH = mul(gCBuf_WorldViewProj, float4(input.vin1st_PosL, 1.0f));
	
	// Color & UV(W) 값 전달.
	output.vout1st_Color = input.vin1st_Color;
	output.vout1st_Tex = input.vin1st_Tex;
	
	//추후에 Material ID가 로직에 들어가면 이 역시 하드코딩에서 바꿔야 한다.
	//output.vout1st_MatID = 0;
	output.vout1st_MatID = input.vin1st_MatID;
	
	return output;
}