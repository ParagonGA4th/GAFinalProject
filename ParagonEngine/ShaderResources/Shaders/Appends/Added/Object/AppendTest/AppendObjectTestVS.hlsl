//Appends VS 필수 조건.

//Append Shader에 쓰일 셰이더 Commons
#include "../../../Libraries/Appends_VSCommon.hlsli"

//다른 헤더들을 추가할 수 있을 것이다.
cbuffer cbAppendsObjectVS : register(b8)
{
    bool toggleNoise; 
};

//반드시 인풋 = VinQuad, 아웃풋 = VOutQuad.
VOutQuad main(VinQuad vin)
{
    VOutQuad vout;
    
    vout.PositionH = float4(vin.Position, 1.0f);
    
    [flatten]
    if (toggleNoise)
    {
        //VS에서의 Material 작동 확인 위해.
        vout.UV = clamp(vin.UV + 0.3f, 0.0f, 1.0f);
    }
    else
    {
        vout.UV = vin.UV;
    }
    
    return vout;
}

