//Appends PS 필수 조건.

//Append Shader에 쓰일 셰이더 Commons
#include "../../../Libraries/Appends_PSCommon.hlsli"
#include "../../../Libraries/MathFunctions/Appends_UVFunctions.hlsli"

//상수버퍼는 무조건 b8에서 시작. 
cbuffer cbAppendsObject : register(b8)
{
    bool isOne;
};

//Texture2D는 무조건 t25에서 시작.
Texture2D<float4> t2_DiffuseTexture : register(t25);
Texture2D<float4> t2_DiffuseTexture2 : register(t26);

//반드시 인풋 = VOutQuad, 아웃풋 = POutQuad
POutQuad main(VOutQuad pin)
{
    //모든 Appends의 Pixel Shader 실행 전에, ClipUnfits 함수가 들어있어야 한다. 구조 특성 때문.
    ClipUnfits(pin.UV);
    
    //본격적인 Shader Code.
    POutQuad res;
    
    [flatten]
    if (isOne)
    {
        res.Output = t2_DiffuseTexture.Sample(defaultTextureSS, GetUV_F2(pin.UV));
    }
    else
    {
        float2 tUVF2 = GetUV_F2(pin.UV);
        tUVF2 = TileUV(tUVF2, float2(1, 1), float2(gCBuf_RadianTimeLoop, 0));
        res.Output = t2_DiffuseTexture2.Sample(defaultTextureSS, tUVF2);
    }
    
    //Pseudo-Fog Test.
    //float fogFactor = saturate(1.0f - GetPseudoLinearDepth(pin.UV));
    //float4 fogColor = 1.0f;
    //res.Output = saturate(fogFactor * res.Output + (1.0 - fogFactor) * fogColor);
        
    return res;
}