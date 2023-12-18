#pragma vertex_shader main
#pragma target 5.0

cbuffer cbPerObject : register(b0)
{
    float4x4 worldMatrix;
    float4x4 viewProjMatrix;
};

struct VertexIn
{
    float3 PosL : POSITION;
    float2 UV : TEXCOORD;
};

struct VertexOut
{
	float3 PosL : POSITION0;
    float3 PosW : POSITION1;
    float4 PosH : SV_POSITION;
    float2 UV : TEXCOORD;
};

VertexOut main(VertexIn vin)
{
    VertexOut vout;
	
    vout.PosL = vin.PosL;
	vout.PosW = mul(worldMatrix, float4(vin.PosL, 1.0f));
    vout.PosH = mul(viewProjMatrix, float4(vout.PosW, 1.0f));
    vout.UV = vin.UV;
    
    return vout;
}