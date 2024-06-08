///그래픽엔진 자체의 1st Vertex Shader (Skinned)

#pragma vertex_shader main
#pragma target 5.0

#include "../../Libraries/System_PerObjectBuffers.hlsli"
#include "../../../Appends/Libraries/SceneInfo/Appends_SceneInfoVSPS.hlsli"
#include "../../Libraries/System_1stLayouts.hlsli"
#include "../../Libraries/System_SwitchingViewProj.hlsli"

VOut1st main(Vin1stPassSkinned_Layout input)
{
    VOut1st output; //= (VOut1st) 0;
	
	  // Calculate the position of the vertex against the world, view, and projection matrices.
   
    float LastWeight = saturate(1.0f - input.vin1st_BlendWeight0 - input.vin1st_BlendWeight1 - input.vin1st_BlendWeight2);
    matrix finalOffsetMatrix = mul(mul(gCBuf_Bones[input.vin1st_BlendIndice0], gCBuf_Nodes[input.vin1st_BlendIndice0]), input.vin1st_BlendWeight0) +
                               mul(mul(gCBuf_Bones[input.vin1st_BlendIndice1], gCBuf_Nodes[input.vin1st_BlendIndice1]), input.vin1st_BlendWeight1) +
                               mul(mul(gCBuf_Bones[input.vin1st_BlendIndice2], gCBuf_Nodes[input.vin1st_BlendIndice2]), input.vin1st_BlendWeight2) +
                               mul(mul(gCBuf_Bones[input.vin1st_BlendIndice3], gCBuf_Nodes[input.vin1st_BlendIndice3]), LastWeight);
	
	
    float3 skinnedPosL = mul(float4(input.vin1st_PosL, 1.0f), finalOffsetMatrix);
	
	//NodeIndex를 쓰지 않는다..?? 일단 보류.
	
	// TODO : offsetMatrix와 node matrix도 곱한 matrix의 역전치를 곱해야될거같은데 일단 보류
	//역전치 밑에서 곱함.
    float3 skinnedNormalL = normalize(mul(float4(input.vin1st_NormalL, 1.0f), finalOffsetMatrix));
    float3 skinnedTangentL = normalize(mul(float4(input.vin1st_TangentL, 1.0f), finalOffsetMatrix));
	
	// Position을 Local -> World 이동.
    output.vout1st_PosW = mul(gCBuf_World, float4(skinnedPosL, 1.0f)).xyz;
	
    // 동차좌표계 내 Position 계산.
    // Direct3D->HLSL에서 Row->ColumnMajor로 들어온 것이기에, WorldViewProj 대신함. (연산 곱 순서 바뀜)
	// WorldViewProj = mul(gCBuf_ViewProjMatrix, gCBuf_World)
    output.vout1st_PosH = mul(mul(gCBuf_SwitchableViewProj, gCBuf_World), float4(skinnedPosL, 1.0f));
	
    output.vout1st_MeshMatID = input.vin1st_MeshMatID;
    output.vout1st_Tex = input.vin1st_Tex;
    output.vout1st_LightmapUV = float2(0, 0); // Skinned는 Lightmap이 허용될 수 없다.
	
	// Normal을 Local -> World 이동.
    output.vout1st_NormalW = mul((float3x3) gCBuf_WorldInvTranspose, skinnedNormalL);
	
	// Tangent를 Local -> World 이동.
    output.vout1st_TangentW = mul((float3x3) gCBuf_WorldInvTranspose, skinnedTangentL);
	
    //무조건 LightPos 기준으로 기록. 일반적으로 그려질 때, 이는 기록될 것이다.
	//output.vout1st_Color = input.vin1st_Color;
    float4 tLightPosH = mul(mul(_lightViewProj, gCBuf_World), float4(skinnedPosL, 1.0f));
    output.vout1st_LightPixelPos = tLightPosH.xyz / tLightPosH.w;
	
    return output;
}

