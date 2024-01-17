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
    //모든 Appends의 Vertex Shader 실행 전에, ClipUnfits 함수가 들어있어야 한다. 구조 특성 때문.
    ClipUnfits(vin.UV);
    
    VOutQuad vout;
    
    vout.PositionH = float4(vin.Position, 1.0f);
    
    [flatten]
    if (toggleNoise)
    {
        vout.UV.x = vin.UV.x + 0.001f;
        vout.UV.y = vin.UV.y + 0.001f;
    }
    else
    {
        vout.UV = vin.UV;
    }
    
    return vout;
}

