TextureCube cubeMap;

SamplerState state
{
    Filter = MIN_POINT_MAG_LINEAR_MIP_POINT;
    AddressU = Wrap;
    AddressV = Wrap;
};

struct VertexOut
{
    float3 PosW : POSITION;
    float4 PosH : SV_POSITION;
    float2 UV : TEXCOORD;
};

float4 PS(VertexOut pin) : SV_Target0
{
    float4 output = cubeMap.Sample(state, pin.PosW);
    //float4 output = float4(1.0f, 0.0f, 0.0f, 1.0f);
    
    return output;
}