cbuffer cbPerObject : register(b0)
{
    float4x4 worldMatrix;
    float4x4 viewMatrix;
    float4x4 projMatrix;
    float4x4 viewProjMatrix;
    float4x4 worldViewProjMatrix;
};

struct VertexIn
{
    float3 PosL : POSITION;
    float2 UV : TEXCOORD;
};

struct VertexOut
{
    float3 PosW : POSITION;
    float4 PosH : SV_POSITION;
    float2 UV : TEXCOORD;
};

VertexOut VS(VertexIn vin)
{
    VertexOut vout;
    
    vout.PosW = vin.PosL;
    vout.PosH = mul(viewProjMatrix, float4(vin.PosL, 1.0f));
    vout.UV = vin.UV;
    
    return vout;
}