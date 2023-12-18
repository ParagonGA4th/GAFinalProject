///그래픽엔진 자체의 1st Vertex Shader (Static)

#pragma vertex_shader main
#pragma target 5.0

#include "../../Libraries/System_PerObjectBuffers.hlsli"
#include "../../Libraries/System_1stLayouts.hlsli"

VOut1st main(Vin1stStatic input)
{
	VOut1st output;
	
	// Position을 Local -> World 이동.
	output.vout1st_PosW = mul(gCBuf_World, float4(input.vin1st_PosL, 1.0f)).xyz;
	
	// Normal을 Local -> World 이동.
	output.vout1st_NormalW = mul((float3x3)gCBuf_WorldInvTranspose, input.vin1st_NormalL);
	
	// Tangent를 Local -> World 이동.
	output.vout1st_TangentW = mul((float3x3)gCBuf_WorldInvTranspose, input.vin1st_TangentL);
	
	// 동차좌표계 내 Position 계산.
	output.vout1st_PosH = mul(gCBuf_WorldViewProj, float4(input.vin1st_PosL, 1.0f));
	//output.vout1st_PosH = mul(gCBuf_WorldView, float4(input.vin1st_PosL, 1.0f));
	
	// Color & UV(W) 값 전달.
	output.vout1st_Color = input.vin1st_Color;
	output.vout1st_Tex = input.vin1st_Tex;
    output.vout1st_LightmapUV = input.vin1st_LightmapUV;
    output.vout1st_Alpha = input.vin1st_Alpha;
	
	//추후에 Object ID가 로직에 들어가면 이 역시 하드코딩에서 바꿔야 한다.
	//output.vin1st_ObjectID = 0;
	output.vout1st_ObjectID = input.vin1st_ObjectID;
	
	return output;
}