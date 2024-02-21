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
	
	//일단은 Depth Bias 문제 해결 위해.
	//float Depth : SV_Depth;
};

PixelOut main(VertexOut pin)
{   
	PixelOut pout;
   // pout.Color = 0.35f;
	
    pout.Color = float4(pin.Color * 0.35f, 1.0f);

	//일단은 Depth Bias 문제 해결 위해.
	//pout.Depth = saturate(pin.Pos.z + 0.001f);
	return pout;
}