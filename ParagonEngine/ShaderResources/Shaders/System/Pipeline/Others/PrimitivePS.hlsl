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
	
	//�ϴ��� Depth Bias ���� �ذ� ����.
	//float Depth : SV_Depth;
};

PixelOut main(VertexOut pin)
{   
	PixelOut pout;
   // pout.Color = 0.35f;
	
    pout.Color = float4(pin.Color * 0.35f, 1.0f);

	//�ϴ��� Depth Bias ���� �ذ� ����.
	//pout.Depth = saturate(pin.Pos.z + 0.001f);
	return pout;
}