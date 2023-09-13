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
	
	float4x4 mat =
	{ 
		0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.5f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	
	float4x4 translate =
	{
		1.0f, 0.0f, 0.0f, 0.2f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.2f,
		0.2f, 0.0f, 0.2f, 1.0f,
	};
	mat = mul(mat, translate);
	
	vout.pos = mul(float4(vin.pos, 1.0f), mat);
	vout.color = vin.color;
	
	return vout;
}