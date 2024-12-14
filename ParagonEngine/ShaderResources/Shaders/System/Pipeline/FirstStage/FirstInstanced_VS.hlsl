///그래픽엔진 자체의 1st Vertex Shader (Static)

#pragma vertex_shader main
#pragma target 5.0

#include "../../Libraries/System_1stLayouts.hlsli"
#include "../../Libraries/System_SwitchingViewProj.hlsli"
#include "../../../Appends/Libraries/SceneInfo/Appends_SceneInfoVSPS.hlsli"

VOut1st_Instanced main(Vin1stPassInstanced_Layout input)
{
    VOut1st_Instanced output;
	
	// Position을 Local -> World 이동.
	// 행렬도 ColumnMajor로 들어온다.
	
	//Row-Major 기준으로 vin1st_Transform이 들어왔다.
	//float4x4 World = Standard;
    float4x4 World = transpose(input.vin1st_Transform);

    output.vout1st_PosW = mul(World, float4(input.vin1st_PosL, 1.0f)).xyz;

	// 동차좌표계 내 Position 계산.
	// Row Major 식으로 곱하기.
    float4x4 WVP = mul(gCBuf_SwitchableViewProj, World);
    output.vout1st_PosH = mul(WVP, float4(input.vin1st_PosL, 1.0f));
	
    output.vout1st_Tex = input.vin1st_Tex;
    output.vout1st_MeshMatID = input.vin1st_MeshMatID;
    output.vout1st_LightmapUV = input.vin1st_LightmapUV;
	
	// Normal을 Local -> World 이동.
	// 만약 Normal이 X,Y,Z가 모두 서로 수직인 좌표계여야 (Shear 등 없어야) 가능한 방법! 
	
	//Row-Major. (Inv-Transpose 안쓰고 Normal을 연산했다. Translate 없애고 Normalize를 해주어서)
    output.vout1st_NormalW = normalize(mul((float3x3) World, input.vin1st_NormalL));
	
	// Tangent를 Local -> World 이동.
    output.vout1st_TangentW = normalize(mul((float3x3) World, input.vin1st_TangentL));
	
	// Color & UV(W) 값 전달.
    //output.vout1st_Color = input.vin1st_Color;
	
	//무조건 LightPos 기준으로 기록. 일반적으로 그려질 때, 이는 기록될 것이다.
    float4 tLightPosH = mul(mul(_lightViewProj, World), float4(input.vin1st_PosL, 1.0f));
    output.vout1st_LightPixelPos = tLightPosH.xyz / tLightPosH.w;
	
	//이제 Instancing 데이터 옮길 차례.
    output.vout1st_ObjID = input.vin1st_ObjID;
    output.vout1st_MatID = input.vin1st_MatID;
    output.vout1st_InstanceID = input.vin1st_InstanceID;
	
    return output;
}