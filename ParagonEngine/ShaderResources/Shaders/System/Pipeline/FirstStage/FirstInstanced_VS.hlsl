///그래픽엔진 자체의 1st Vertex Shader (Static)

#pragma vertex_shader main
#pragma target 5.0

#include "../../Libraries/System_1stLayouts.hlsli"
#include "../../../Appends/Libraries/SceneInfo/Appends_SceneInfoPS.hlsli"

VOut1st_Instanced main(Vin1stPassInstanced_Layout input)
{
    VOut1st_Instanced output;
	
	// Position을 Local -> World 이동.
	// 행렬도 ColumnMajor로 들어온다.
	
	//Row-Major 기준으로 vin1st_Transform이 들어왔다.
    output.vout1st_PosW = mul(float4(input.vin1st_PosL, 1.0f), input.vin1st_Transform).xyz;

	// 동차좌표계 내 Position 계산.
	// Row Major 식으로 곱하기.
    output.vout1st_PosH = mul(float4(input.vin1st_PosL, 1.0f), mul(input.vin1st_Transform, transpose(gCBuf_ViewProjMatrix)));
	
    output.vout1st_Tex = input.vin1st_Tex;
    output.vout1st_MeshMatID = input.vin1st_MeshMatID;
    output.vout1st_LightmapUV = input.vin1st_LightmapUV;
	
	// Normal을 Local -> World 이동.
	// 만약 Normal이 X,Y,Z가 모두 서로 수직인 좌표계여야 (Shear 등 없어야) 가능한 방법! 
	
	//Row-Major. (Inv-Transpose 안쓰고 Normal을 연산했다. Translate 없애고 Normalize를 해주어서)
    output.vout1st_NormalW = normalize(mul(input.vin1st_NormalL, (float3x3) input.vin1st_Transform));
	
	// Tangent를 Local -> World 이동.
    output.vout1st_TangentW = normalize(mul(input.vin1st_TangentL, (float3x3) input.vin1st_Transform));
	
	// Color & UV(W) 값 전달.
	output.vout1st_Color = input.vin1st_Color;
	
	//이제 Instancing 데이터 옮길 차례.
    output.vout1st_ObjID = input.vin1st_ObjID;
    output.vout1st_MatID = input.vin1st_MatID;
    output.vout1st_InstanceID = input.vin1st_InstanceID;
	
	return output;
}