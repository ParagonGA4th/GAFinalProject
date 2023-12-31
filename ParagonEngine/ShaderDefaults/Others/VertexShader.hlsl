cbuffer cbPerObject : register(b0)
{
	float4x4 worldMatrix;
	float4x4 viewMatrix;
	float4x4 projMatrix;
	float4x4 viewProjMatrix;
    float3 eyePos;
};

struct VertexIn
{
	float3 PosL : POSITION;
	float3 Color : COLOR;
};

struct VertexOut
{
    float3 PosW : POSITION;
    float4 PosH : SV_POSITION;
    float3 Color : COLOR;
    float3 Normal : NORMAL;
};

VertexOut main(VertexIn vin)
{
	VertexOut vout;
	
	float4 vec = float4(vin.PosL, 1.0f);
	
	float4x4 WVP = mul(viewProjMatrix, worldMatrix);
	
	vout.PosH = mul(WVP, vec);
	vout.Color = vin.Color;
	
	return vout;
}