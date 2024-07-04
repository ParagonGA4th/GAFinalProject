#pragma pixel_shader main
#pragma target 5.0

TextureCube cubeMap : register(t0);
SamplerState state : register(s0);

struct VertexOut
{
	float3 PosL : POSITION0;
    float3 PosW : POSITION1;
    float4 PosH : SV_POSITION;
    float2 UV : TEXCOORD;
};

float4 main(VertexOut pin) : SV_Target0
{
    float4 output = cubeMap.Sample(state, pin.PosL);
    
    return output;
}