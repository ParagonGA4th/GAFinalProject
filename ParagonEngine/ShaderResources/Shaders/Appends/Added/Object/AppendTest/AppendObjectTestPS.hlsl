//Appends PS 필수 조건.

//Append Shader에 쓰일 셰이더 Commons
#include "../../../Libraries/Appends_PSCommon.hlsli"

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
    //모든 PS 셰이더 실행 전에, ClipUnfits 함수를 실행해줘야 한다. 구조 특성 때문.
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
        res.Output = t2_DiffuseTexture2.Sample(defaultTextureSS, GetUV_F2(pin.UV));
    }
        
    return res;
}