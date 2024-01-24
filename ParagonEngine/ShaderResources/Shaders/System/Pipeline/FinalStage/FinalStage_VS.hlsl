//SystemQuad에서 MainRenderTarget으로 뿌려주는 역할. (VS)
#include "../../Libraries/System_QuadLayouts.hlsli"

//반드시 인풋 = VinQuad, 아웃풋 = VOutQuad.
VOutQuad main(VinQuad vin)
{
    VOutQuad vout;
    
    vout.PositionH = float4(vin.Position, 1.0f);
    vout.UV = vin.UV;
	
    return vout;
}