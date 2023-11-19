TextureCube cubeMap;

cbuffer cbPerObject : register(b0)
{
    float4x4 worldMatrix;
    float4x4 viewMatrix;
    float4x4 projMatrix;
    float4x4 viewProjMatrix;
    float3 eyePos;
};

SamplerState state
{
    Filter = MIN_POINT_MAG_LINEAR_MIP_POINT;
    AddressU = Wrap;
    AddressV = Wrap;
};

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
    // 환경 맵핑 코드
    // float3 view = (pin.posW - eyePos);
    // float3 reflect = reflect(view, normalize(pin.normal);
    // float3 cube = view + reflect;
    // float4 cubemapColor = cubeMap.Sample(state, cube);
    
	PixelOut pout;
	pout.Color = float4(pin.Color, 1.0f);

	return pout;
}