cbuffer cbPerObject : register(b0)
{
	float degree;
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
	
	float transX = 0.0f;
	float transY = 0.0f;
	float transZ = 0.0f;
	
	float scale = 0.5f;
	
	float Yaw = degree * 10000.f;
	float Pitch = 0.f;
	float Roll = degree * 10000.f;
	

	float4x4 mat =
	{
		scale, 0.0f, 0.0f, 0.0f,
		0.0f, scale, 0.0f, 0.0f,
		0.0f, 0.0f, scale, 0.0f,
		transX, transY, transZ, 1.0f
	};
	

	float4x4 yaw =
	{
		cos(Yaw), 0.0f, -sin(Yaw), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		sin(Yaw), 0.0f, cos(Yaw), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
		
	};
	
	float4x4 pitch =
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cos(Pitch), -sin(Pitch), 0.0f,
		0.0f, sin(Pitch), cos(Pitch), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	
	float4x4 roll =
	{
		cos(Roll), -sin(Roll), 0.0f, 0.0f,
		sin(Roll), cos(Roll), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	
	mat = mul(mat, yaw);
	mat = mul(mat, pitch);
	mat = mul(mat, roll);
	//mat = mul(mat, viewMatrix);
	//mat = mul(mat, projMatrix);
	mat = mul(mat, viewProjMatrix);

	float4 vec = float4(vin.pos, 1.0f);
	
	vout.pos = normalize(mul(vec, mat));
	vout.color = vin.color;
	
	return vout;
}