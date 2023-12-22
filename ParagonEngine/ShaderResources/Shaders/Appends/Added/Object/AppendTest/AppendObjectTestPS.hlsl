//Appends PS 필수 조건.

//Append Shader에 쓰일 셰이더 Commons
#include "../../../Libraries/Appends_PSCommon.hlsli"

//상수버퍼는 무조건 b8에서 시작. 
cbuffer cbAppendsObject : register(b8)
{
    bool isOne;
   //float3 test1;
   //int test2;
   //uint test3;
   //float4 test4;
   //float2 test5;
   //float3 test6;
   //int test7;
};

//Texture2D는 무조건 t25에서 시작.
Texture2D<float4> t2_DiffuseTexture : register(t25);
Texture2D<float4> t2_DiffuseTexture2 : register(t26);

//반드시 인풋 = VOutQuad, 아웃풋 = POutQuad
POutQuad main(VOutQuad pin)
{
    //일단 
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