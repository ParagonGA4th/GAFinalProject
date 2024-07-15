//Appends VS 필수 조건.

//Append Shader에 쓰일 셰이더 Commons
#include "../../../Libraries/Appends_VSCommon.hlsli"

//반드시 인풋 = VinQuad, 아웃풋 = VOutQuad.
VOutQuad main(VinQuad vin)
{   
    VOutQuad vout;
    
    vout.PositionH = float4(vin.Position, 1.0f);
    vout.UV = vin.UV;
    
    return vout;
}

