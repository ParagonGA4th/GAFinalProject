#pragma vertex_shader main
#pragma target 5.0

//Edited By Taewook Koh, From BasisHLSL11_VS.hlsl.
//-> Multimaterial Mesh�� ���� ����. (Skinned Input Layout�� ������, ���� ����X)
//����μ��� Diffuse�� �̿� �ش�Ǵ� ������� �۵�.

//Constant Buffers
cbuffer cbPerObjectBase : register(b0)
{
    float4x4 gCBuf_World;
    float4x4 gCBuf_WorldInvTranspose;
    float4x4 gCBuf_WorldView;
    float4x4 gCBuf_WorldViewProj;
    float3 gCBuf_CameraPositionW;
    // gCBuf_Materials[10] //�Ŀ� Material�� ���� �߰��Ǿ��.
};

cbuffer cbPerObjectSkinned : register(b1)
{
    //������ ��ũ�η� ���� ����.
    float4x4 gCBuf_Bones[100];
};


//I/O
struct Vin1stSkinned
{
    float3 vin1st_PosL : POSITION;
    float3 vin1st_NormalL : NORMAL;
    float3 vin1st_TangentL : TANGENT;
    float4 vin1st_Color : COLOR;
    float3 vin1st_Tex : TEXCOORD;
    uint vin1st_MatID : MATERIALID;
    
    uint vin1st_BlendIndice0 : BLENDINDICES0;
    uint vin1st_BlendIndice1 : BLENDINDICES1;
    uint vin1st_BlendIndice2 : BLENDINDICES2;
    uint vin1st_BlendIndice3 : BLENDINDICES3;
    
    float vin1st_BlendWeight0 : BLENDWEIGHT0;
    float vin1st_BlendWeight1 : BLENDWEIGHT1;
    float vin1st_BlendWeight2 : BLENDWEIGHT2;
};

struct VOut1st
{
    float4 vout1st_PosH : SV_POSITION;
    float3 vout1st_PosW : POSITION;
    float3 vout1st_NormalW : NORMAL;
    float3 vout1st_TangentW : TANGENT;
    float4 vout1st_Color : COLOR;
    float3 vout1st_Tex : TEXCOORD;
    uint vout1st_MatID : MATERIALID;
};

VOut1st main(Vin1stSkinned input)
{   
    VOut1st output;
	
	//Skinning ����.
    float _weights[4] = { 0.f, 0.f, 0.f, 0.f };
    _weights[0] = input.vin1st_BlendWeight0;
    _weights[1] = input.vin1st_BlendWeight1;
    _weights[2] = input.vin1st_BlendWeight2;
    _weights[3] = 1 - (_weights[0] + _weights[1] + _weights[2]);

    float3 skinnedPosL = float3(0.f, 0.f, 0.f);
    float3 skinnedNormalL = float3(0.f, 0.f, 0.f);
    float3 skinnedTangentL = float3(0.f, 0.f, 0.f);
	
	//231030 : Tangent �߰�.
	
    skinnedPosL += _weights[0] * mul(float4(input.vin1st_PosL, 1.f), gCBuf_Bones[input.vin1st_BlendIndice0]).xyz;
    skinnedNormalL += _weights[0] * mul(input.vin1st_NormalL, (float3x3) gCBuf_Bones[input.vin1st_BlendIndice0]);
    skinnedTangentL += _weights[0] * mul(input.vin1st_TangentL, (float3x3) gCBuf_Bones[input.vin1st_BlendIndice0]);
	
    skinnedPosL += _weights[1] * mul(float4(input.vin1st_PosL, 1.f), gCBuf_Bones[input.vin1st_BlendIndice1]).xyz;
    skinnedNormalL += _weights[1] * mul(input.vin1st_NormalL, (float3x3) gCBuf_Bones[input.vin1st_BlendIndice1]);
    skinnedTangentL += _weights[1] * mul(input.vin1st_TangentL, (float3x3) gCBuf_Bones[input.vin1st_BlendIndice1]);
	
    skinnedPosL += _weights[2] * mul(float4(input.vin1st_PosL, 1.f), gCBuf_Bones[input.vin1st_BlendIndice2]).xyz;
    skinnedNormalL += _weights[2] * mul(input.vin1st_NormalL, (float3x3) gCBuf_Bones[input.vin1st_BlendIndice2]);
    skinnedTangentL += _weights[2] * mul(input.vin1st_TangentL, (float3x3) gCBuf_Bones[input.vin1st_BlendIndice2]);
	
    skinnedPosL += _weights[3] * mul(float4(input.vin1st_PosL, 1.f), gCBuf_Bones[input.vin1st_BlendIndice3]).xyz;
    skinnedNormalL += _weights[3] * mul(input.vin1st_NormalL, (float3x3) gCBuf_Bones[input.vin1st_BlendIndice3]);
    skinnedTangentL += _weights[3] * mul(input.vin1st_TangentL, (float3x3) gCBuf_Bones[input.vin1st_BlendIndice3]);
	
	// Position�� Local -> World �̵�.
    output.vout1st_PosW = mul(float4(skinnedPosL, 1.0f), gCBuf_World).xyz;
	
	// Normal�� Local -> World �̵�.
    output.vout1st_NormalW = mul(skinnedNormalL, (float3x3) gCBuf_WorldInvTranspose);
	
	// Tangent�� Local -> World �̵�.
    output.vout1st_TangentW = mul(skinnedTangentL, (float3x3) gCBuf_World);
	
	// ������ǥ�� �� Position ���.
    //�� ���밡����.. �̰� SkinnedPos�� �� ��ٰ�...??? 
    output.vout1st_PosH = mul(gCBuf_WorldViewProj, float4(skinnedPosL, 1.0f));
    
	// Color & UV(W) �� ����.
    output.vout1st_Color = input.vin1st_Color;
    output.vout1st_Tex = input.vin1st_Tex;
	
	//���Ŀ� Material ID�� ������ ���� �� ���� �ϵ��ڵ����� �ٲ�� �Ѵ�.
    //output.vout1st_MatID = 0;
    //output.vout1st_MatID = input.vin1st_MatID; //�̰� ��¥.
    
    output.vout1st_MatID = input.vin1st_MatID;
    return output;
}
