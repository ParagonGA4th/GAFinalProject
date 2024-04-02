///그래픽엔진 자체의 1st Vertex Shader (Skinned)

#pragma vertex_shader main
#pragma target 5.0

#include "../../Libraries/System_PerObjectBuffers.hlsli"
#include "../../Libraries/System_1stLayouts.hlsli"

VOut1st main(Vin1stSkinned input)
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
    float3 skinnedNormalL = normalize(mul(float4(input.vin1st_NormalL, 0.0f), finalOffsetMatrix));
    float3 skinnedTangentL = normalize(mul(float4(input.vin1st_TangentL, 0.0f), finalOffsetMatrix));
	
	// Position을 Local -> World 이동.
    output.vout1st_PosW = mul(float4(skinnedPosL, 1.0f), gCBuf_World).xyz;
	
    // 동차좌표계 내 Position 계산.
    output.vout1st_PosH = mul(float4(skinnedPosL, 1.0f), gCBuf_WorldViewProj);
	
	// Alpha
    output.vout1st_Alpha = input.vin1st_Alpha;
	
	// Normal을 Local -> World 이동.
	output.vout1st_NormalW = mul(skinnedNormalL, (float3x3) gCBuf_WorldInvTranspose);
	
	// Tangent를 Local -> World 이동.
    output.vout1st_TangentW = mul(skinnedTangentL, (float3x3) gCBuf_WorldInvTranspose);
	
	output.vout1st_Color = input.vin1st_Color;
	output.vout1st_MeshMatID = input.vin1st_MeshMatID;
	output.vout1st_Tex = input.vin1st_Tex;
    output.vout1st_LightmapUV = float2(0, 0); // Skinned는 Lightmap이 허용될 수 없다.
    
    return output;
}

