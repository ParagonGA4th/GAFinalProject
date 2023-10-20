SamplerState state
{
	Filter = MIN_POINT_MAG_LINEAR_MIP_POINT;
	AddressU = Wrap;
	AddressV = Wrap;
};

struct VertexOut
{
	float4 PositionH : SV_POSITION;
	float2 UV : TEXCOORD;
};

Texture2D GBuffer[6];

float4 main(VertexOut pin) : SV_TARGET
{
	float4 output;
	
	output = GBuffer[3].Sample(state, pin.UV);

	return output;
}