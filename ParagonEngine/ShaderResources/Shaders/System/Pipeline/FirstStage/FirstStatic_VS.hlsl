///그래픽엔진 자체의 1st Vertex Shader (Static)

#pragma vertex_shader main
#pragma target 5.0

#include "../../Libraries/System_PerObjectBuffers.hlsli"
#include "../../../Appends/Libraries/SceneInfo/Appends_SceneInfoVSPS.hlsli"
#include "../../Libraries/System_1stLayouts.hlsli"
#include "../../Libraries/System_SwitchingViewProj.hlsli"

VOut1st main(Vin1stPassStatic_Layout input)
{
	VOut1st output;
	
	// Position을 Local -> World 이동.
	output.vout1st_PosW = mul(gCBuf_World, float4(input.vin1st_PosL, 1.0f)).xyz;
	// 동차좌표계 내 Position 계산.
	//Direct3D->HLSL에서 Row->ColumnMajor로 들어온 것이기에, WorldViewProj 대신함. (연산 곱 순서 바뀜)
	//WorldViewProj = mul(gCBuf_ViewProjMatrix, gCBuf_World)
    output.vout1st_PosH = mul(mul(gCBuf_SwitchableViewProj, gCBuf_World), float4(input.vin1st_PosL, 1.0f));
	
    output.vout1st_MeshMatID = input.vin1st_MeshMatID;
    output.vout1st_Tex = input.vin1st_Tex;
    output.vout1st_LightmapUV = input.vin1st_LightmapUV;
	
	// Normal을 Local -> World 이동.
	output.vout1st_NormalW = mul((float3x3)gCBuf_WorldInvTranspose, input.vin1st_NormalL);
	
	// Tangent를 Local -> World 이동.
	output.vout1st_TangentW = mul((float3x3)gCBuf_WorldInvTranspose, input.vin1st_TangentL);
	
	// Color & UV(W) 값 전달.
	//output.vout1st_Color = input.vin1st_Color;
	
	//무조건 LightPos 기준으로 기록. 일반적으로 그려질 때, 이는 기록될 것이다.
    float4 tLightPosH = mul(mul(_lightViewProj, gCBuf_World), float4(input.vin1st_PosL, 1.0f));
    output.vout1st_LightPixelPos = tLightPosH.xyz / tLightPosH.w;
	
	return output;
}