TextureCube cubeMap;

SamplerState state
{
    Filter = MIN_POINT_MAG_LINEAR_MIP_POINT;
    AddressU = Wrap;
    AddressV = Wrap;
};

struct VertexOut
{
	float3 PosL : POSITION0;
    float3 PosW : POSITION1;
    float4 PosH : SV_POSITION;
    float2 UV : TEXCOORD;
};

float4 PS(VertexOut pin) : SV_Target0
{
    float4 output = cubeMap.Sample(state, pin.PosL);
	
    return output;
}