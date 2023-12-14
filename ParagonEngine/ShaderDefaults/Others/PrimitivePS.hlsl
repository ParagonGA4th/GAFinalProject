struct VertexOut
{
    float3 PosW : POSITION;
	float4 Pos : SV_POSITION;
	float3 Color : COLOR;
    float3 Normal : NORMAL;
};

struct PixelOut
{
	float4 Color : SV_Target0;
};

PixelOut PS(VertexOut pin)
{   
	PixelOut pout;
	pout.Color = float4(pin.Color, 1.0f);

	return pout;
}