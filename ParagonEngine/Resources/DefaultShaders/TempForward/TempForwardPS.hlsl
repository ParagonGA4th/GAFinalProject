//231011 Edited By Taewook Koh, From BasisHLSL11_PS.hlsl.
//Multimaterial Mesh를 위해서 개조.
//현재로서는 Diffuse만 이에 해당되는 방식으로 작동.

cbuffer cbPerObjectBase : register(b0)
{
    float4x4 gCBuf_World;
    float4x4 gCBuf_WorldInvTranspose;
    float4x4 gCBuf_WorldViewProj;
    float3 gCBuf_CameraPositionW;
    // gCBuf_Materials[10] //후에 Material이 들어가면 추가되어야.
}

//텍스쳐 & Samplers
Texture2D g_txDiffuse : register(t0);
SamplerState g_samLinear : register(s0);

//I/O
struct VOut1st
{
    float4 vout1st_PosH : SV_POSITION;
    float3 vout1st_PosW : POSITION;
    float3 vout1st_NormalW : NORMAL;
    float3 vout1st_TangentW : TANGENT;
    float4 vout1st_Color : COLOR;
    float3 vout1st_Tex : TEXCOORD;
    uint vout1st_MatID : MATERIALID;
};

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS_MAIN(VOut1st input) : SV_TARGET
{
    //float4 vDiffuse = g_txDiffuse.Sample(g_samLinear, float2(input.vout1st_Tex.x, input.vout1st_Tex.y));
	//
    //float fLighting = saturate(dot(g_vLightDir, Input.vNormal));
    //fLighting = max(fLighting, g_fAmbient);
	//
    //return vDiffuse * fLighting;
    return float4(1.0f, 1.0f, 1.0f, 1.0f);

}

