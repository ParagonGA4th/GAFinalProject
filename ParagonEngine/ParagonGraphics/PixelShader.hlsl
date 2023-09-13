struct VertexOut
{
	float4 pos : SV_POSITION;
	float3 color : COLOR;
};

struct PixelOut
{
	float4 color : SV_Target0;
};

PixelOut PS(VertexOut pin)
{
	PixelOut pout;
	pout.color = float4(pin.color, 0.5f);
	//pout.color = float4(1.0f, 1.0f, 0.0f, 0.0f);

	return pout;
}