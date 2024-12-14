//Skinned Default Alpha.

#pragma vertex_shader main
#pragma target 5.0

#include "../../Libraries/System_PerObjectBuffers.hlsli"
#include "../../../Appends/Libraries/SceneInfo/Appends_SceneInfoVSPS.hlsli"
#include "../../Libraries/System_1stLayouts.hlsli"
#include "../../Libraries/System_AlphaDefaultInfo.hlsli"

VOut1st main(Vin1stPassSkinned_Layout input)
{
    VOut1st output; //= (VOut1st) 0;
	
	  // Calculate the position of the vertex against the world, view, and projection matrices.
   
    //float LastWeight = saturate(1.0f - input.vin1st_BlendWeight0 - input.vin1st_BlendWeight1 - input.vin1st_BlendWeight2);
    matrix finalOffsetMatrix = mul(mul(gCBuf_Bones[input.vin1st_BlendIndice0], gCBuf_Nodes[input.vin1st_BlendIndice0]), input.vin1st_BlendWeight0) +
                               mul(mul(gCBuf_Bones[input.vin1st_BlendIndice1], gCBuf_Nodes[input.vin1st_BlendIndice1]), input.vin1st_BlendWeight1) +
                               mul(mul(gCBuf_Bones[input.vin1st_BlendIndice2], gCBuf_Nodes[input.vin1st_BlendIndice2]), input.vin1st_BlendWeight2) +
                               mul(mul(gCBuf_Bones[input.vin1st_BlendIndice3], gCBuf_Nodes[input.vin1st_BlendIndice3]), input.vin1st_BlendWeight3);
	
    float4 skinnedPosL = mul(float4(input.vin1st_PosL, 1.0f), finalOffsetMatrix);
    skinnedPosL /= skinnedPosL.w;
    
	//NodeIndex�� ���� �ʴ´�..?? �ϴ� ����.
	
	// TODO : offsetMatrix�� node matrix�� ���� matrix�� ����ġ�� ���ؾߵɰŰ����� �ϴ� ����
	//����ġ �ؿ��� ����.
    float3 skinnedNormalL = normalize(mul(float4(input.vin1st_NormalL, 1.0f), finalOffsetMatrix));
    float3 skinnedTangentL = normalize(mul(float4(input.vin1st_TangentL, 1.0f), finalOffsetMatrix));
	
	// Position�� Local -> World �̵�.
    //output.vout1st_PosW = mul(gCBuf_World, float4(skinnedPosL, 1.0f)).xyz;
    output.vout1st_PosW = mul(gCBuf_World, skinnedPosL).xyz;
	
    // ������ǥ�� �� Position ���.
    // Direct3D->HLSL���� Row->ColumnMajor�� ���� ���̱⿡, WorldViewProj �����. (���� �� ���� �ٲ�)
	// WorldViewProj = mul(gCBuf_ViewProjMatrix, gCBuf_World)
    output.vout1st_PosH = mul(mul(gCBuf_CameraViewProj, gCBuf_World), skinnedPosL);
	
    output.vout1st_MeshMatID = input.vin1st_MeshMatID;
    output.vout1st_Tex = input.vin1st_Tex;
    output.vout1st_LightmapUV = float2(0, 0); // Skinned�� Lightmap�� ���� �� ����.
	
	// Normal�� Local -> World �̵�.
    output.vout1st_NormalW = mul((float3x3) gCBuf_WorldInvTranspose, skinnedNormalL);
	
	// Tangent�� Local -> World �̵�.
    output.vout1st_TangentW = mul((float3x3) gCBuf_WorldInvTranspose, skinnedTangentL);
	
    //������ LightPos �������� ���. �Ϲ������� �׷��� ��, �̴� ��ϵ� ���̴�.
	//output.vout1st_Color = input.vin1st_Color;
    float4 tLightPosH = mul(mul(_lightViewProj, gCBuf_World), skinnedPosL);
    output.vout1st_LightPixelPos = tLightPosH.xyz / tLightPosH.w;
	
    return output;
}

