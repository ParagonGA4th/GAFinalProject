//SamplerState state;

#include "../Libraries/DefaultBufferPerObject.hlsli"

struct VertexIn
{
	float3 Position : POSITION;
	float3 Nomral : NORMAL;
	float2 UV : TEXCOORD;
};

struct VertexOut
{
	float4 PositionH : SV_POSITION;
	float2 UV : TEXCOORD;
};

Texture2D GBuffer[6];

VertexOut main(VertexIn vin)
{
	VertexOut vout;

	vout.PositionH = float4(vin.Position, 1.0f);
	vout.UV = vin.UV;
	
	return vout;
}