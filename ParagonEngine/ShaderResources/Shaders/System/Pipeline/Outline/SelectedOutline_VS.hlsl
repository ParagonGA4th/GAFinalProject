#include "../../Libraries/System_QuadLayouts.hlsli"

///�ʿ��� ��Ҹ� �ɷ��� �������� ����ϴ� Shader. (VS)

//�ݵ�� ��ǲ = VinQuad, �ƿ�ǲ = VOutQuad.
VOutQuad main(VinQuad vin)
{
    VOutQuad vout;
    
    vout.PositionH = float4(vin.Position, 1.0f);
    vout.UV = vin.UV;
	
    return vout;
}