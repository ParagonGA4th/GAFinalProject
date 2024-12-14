#pragma vertex_shader main
#pragma target 5.0

cbuffer cbPerObject : register(b0)
{
    float4x4 worldMatrix;
    float4x4 viewProjMatrix;
    float3 camPos;
};

static const float maxDistance = 100.0f;

struct VertexIn
{
    float3 PosL : POSITION;
    float3 Color : COLOR;
};

struct VertexOut
{
    float3 PosW : POSITION;
    float4 PosH : SV_POSITION;
    float3 Color : COLOR;
    float3 Normal : NORMAL;
};

VertexOut main(VertexIn vin)
{
    VertexOut vout;
	
    float4 vec = float4(vin.PosL, 1.0f);
	
    float4x4 WVP = mul(viewProjMatrix, worldMatrix);
    vout.PosW = mul(float4(vin.PosL, 1.0f), worldMatrix).xyz;
    vout.PosH = mul(WVP, vec);
	
	//최대 거리를 구한다.
    //float tLength = length(vout.PosW - camPos);
    //tLength = min(maxDistance, tLength);
    //float tRatio = saturate(tLength / maxDistance);
    //vout.Color = mul(vin.Color, tRatio);
    vout.Color = vin.Color;
 
    return vout;
}