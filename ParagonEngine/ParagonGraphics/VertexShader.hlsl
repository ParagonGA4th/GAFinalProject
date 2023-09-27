cbuffer cbPerObject : register(b0)
{
	float4x4 worldMatrix;
	float4x4 viewMatrix;
	float4x4 projMatrix;
	float4x4 viewProjMatrix;
};

struct VertexIn
{
	float3 pos : POSITION;
	float3 color : COLOR;
};

struct VertexOut
{
	float4 pos : SV_POSITION;
	float3 color : COLOR;
};

VertexOut main(VertexIn vin)
{
	VertexOut vout;
	
	float4 vec = float4(vin.pos, 1.0f);
	
	float4x4 WVP = mul(viewProjMatrix, worldMatrix);
	
	vout.pos = mul(WVP, vec);
	vout.color = vin.color;
	
	return vout;
}