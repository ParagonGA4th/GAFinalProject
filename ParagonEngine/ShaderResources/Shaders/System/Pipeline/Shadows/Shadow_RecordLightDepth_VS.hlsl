#include "../../Libraries/System_QuadLayouts.hlsli"
#include "../../../Appends/Libraries/SceneInfo/Appends_SceneInfoVSPS.hlsli"
///ShadowMapping�� ���� ����ϴ� Shader.

//�ݵ�� ��ǲ = VinQuad, �ƿ�ǲ = VOutQuad.
VOutQuad main(VinQuad vin)
{
    VOutQuad vout;
    
    vout.PositionH = float4(vin.Position, 1.0f);
    vout.UV = vin.UV;
	//PS���� ���ø��� ���� ������ �����ϱ�!
    
    return vout;
}