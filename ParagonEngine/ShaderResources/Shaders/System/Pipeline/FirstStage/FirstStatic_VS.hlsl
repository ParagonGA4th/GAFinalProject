///�׷��ȿ��� ��ü�� 1st Vertex Shader (Static)

#pragma vertex_shader main
#pragma target 5.0

#include "../../Libraries/System_PerObjectBuffers.hlsli"
#include "../../../Appends/Libraries/SceneInfo/Appends_SceneInfoVSPS.hlsli"
#include "../../Libraries/System_1stLayouts.hlsli"
#include "../../Libraries/System_SwitchingViewProj.hlsli"

VOut1st main(Vin1stPassStatic_Layout input)
{
	VOut1st output;
	
	// Position�� Local -> World �̵�.
	output.vout1st_PosW = mul(gCBuf_World, float4(input.vin1st_PosL, 1.0f)).xyz;
	// ������ǥ�� �� Position ���.
	//Direct3D->HLSL���� Row->ColumnMajor�� ���� ���̱⿡, WorldViewProj �����. (���� �� ���� �ٲ�)
	//WorldViewProj = mul(gCBuf_ViewProjMatrix, gCBuf_World)
    output.vout1st_PosH = mul(mul(gCBuf_SwitchableViewProj, gCBuf_World), float4(input.vin1st_PosL, 1.0f));
	
    output.vout1st_MeshMatID = input.vin1st_MeshMatID;
    output.vout1st_Tex = input.vin1st_Tex;
    output.vout1st_LightmapUV = input.vin1st_LightmapUV;
	
	// Normal�� Local -> World �̵�.
	output.vout1st_NormalW = mul((float3x3)gCBuf_WorldInvTranspose, input.vin1st_NormalL);
	
	// Tangent�� Local -> World �̵�.
	output.vout1st_TangentW = mul((float3x3)gCBuf_WorldInvTranspose, input.vin1st_TangentL);
	
	// Color & UV(W) �� ����.
	//output.vout1st_Color = input.vin1st_Color;
	
	//������ LightPos �������� ���. �Ϲ������� �׷��� ��, �̴� ��ϵ� ���̴�.
    float4 tLightPosH = mul(mul(_lightViewProj, gCBuf_World), float4(input.vin1st_PosL, 1.0f));
    output.vout1st_LightPixelPos = tLightPosH.xyz / tLightPosH.w;
	
	return output;
}