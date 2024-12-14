#include "../../Libraries/System_QuadLayouts.hlsli"

///ShadowMapping을 위해 사용하는 Shader.

//반드시 인풋 = VinQuad, 아웃풋 = VOutQuad.
VOutQuad main(VinQuad vin)
{
    VOutQuad vout;
    
    vout.PositionH = float4(vin.Position, 1.0f);
    vout.UV = vin.UV;
	
    return vout;
}