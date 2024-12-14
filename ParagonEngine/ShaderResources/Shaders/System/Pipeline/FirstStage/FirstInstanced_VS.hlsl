///�׷��ȿ��� ��ü�� 1st Vertex Shader (Static)

#pragma vertex_shader main
#pragma target 5.0

#include "../../Libraries/System_1stLayouts.hlsli"
#include "../../Libraries/System_SwitchingViewProj.hlsli"
#include "../../../Appends/Libraries/SceneInfo/Appends_SceneInfoVSPS.hlsli"

VOut1st_Instanced main(Vin1stPassInstanced_Layout input)
{
    VOut1st_Instanced output;
	
	// Position�� Local -> World �̵�.
	// ��ĵ� ColumnMajor�� ���´�.
	
	//Row-Major �������� vin1st_Transform�� ���Դ�.
	//float4x4 World = Standard;
    float4x4 World = transpose(input.vin1st_Transform);

    output.vout1st_PosW = mul(World, float4(input.vin1st_PosL, 1.0f)).xyz;

	// ������ǥ�� �� Position ���.
	// Row Major ������ ���ϱ�.
    float4x4 WVP = mul(gCBuf_SwitchableViewProj, World);
    output.vout1st_PosH = mul(WVP, float4(input.vin1st_PosL, 1.0f));
	
    output.vout1st_Tex = input.vin1st_Tex;
    output.vout1st_MeshMatID = input.vin1st_MeshMatID;
    output.vout1st_LightmapUV = input.vin1st_LightmapUV;
	
	// Normal�� Local -> World �̵�.
	// ���� Normal�� X,Y,Z�� ��� ���� ������ ��ǥ�迩�� (Shear �� �����) ������ ���! 
	
	//Row-Major. (Inv-Transpose �Ⱦ��� Normal�� �����ߴ�. Translate ���ְ� Normalize�� ���־)
    output.vout1st_NormalW = normalize(mul((float3x3) World, input.vin1st_NormalL));
	
	// Tangent�� Local -> World �̵�.
    output.vout1st_TangentW = normalize(mul((float3x3) World, input.vin1st_TangentL));
	
	// Color & UV(W) �� ����.
    //output.vout1st_Color = input.vin1st_Color;
	
	//������ LightPos �������� ���. �Ϲ������� �׷��� ��, �̴� ��ϵ� ���̴�.
    float4 tLightPosH = mul(mul(_lightViewProj, World), float4(input.vin1st_PosL, 1.0f));
    output.vout1st_LightPixelPos = tLightPosH.xyz / tLightPosH.w;
	
	//���� Instancing ������ �ű� ����.
    output.vout1st_ObjID = input.vin1st_ObjID;
    output.vout1st_MatID = input.vin1st_MatID;
    output.vout1st_InstanceID = input.vin1st_InstanceID;
	
    return output;
}