#include "../../Libraries/System_QuadLayouts.hlsli"

///필요한 요소만 걸러낼 목적으로 사용하는 Shader. (VS)

//반드시 인풋 = VinQuad, 아웃풋 = VOutQuad.
VOutQuad main(VinQuad vin)
{
    VOutQuad vout;
    
    vout.PositionH = float4(vin.Position, 1.0f);
    vout.UV = vin.UV;
	
    return vout;
}