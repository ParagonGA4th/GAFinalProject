//SystemQuad���� MainRenderTarget���� �ѷ��ִ� ����. (VS)
#include "../../Libraries/System_QuadLayouts.hlsli"

//�ݵ�� ��ǲ = VinQuad, �ƿ�ǲ = VOutQuad.
VOutQuad main(VinQuad vin)
{
    VOutQuad vout;
    
    vout.PositionH = float4(vin.Position, 1.0f);
    vout.UV = vin.UV;
	
    return vout;
}