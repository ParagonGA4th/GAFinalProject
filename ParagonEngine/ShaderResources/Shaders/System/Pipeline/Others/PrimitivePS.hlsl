#pragma pixel_shader main
#pragma target 5.0

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

PixelOut main(VertexOut pin)
{   
	PixelOut pout;
	pout.Color = float4(pin.Color, 1.f);

	return pout;
}