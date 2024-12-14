//�ʿ��� ��Ҹ� ��� ���������� ������ ����.

#include "../../Libraries/System_QuadLayouts.hlsli"
#include "../../../Appends/Libraries/Appends_PSCommon.hlsli"
#include "../../../Appends/Libraries/SamplerStates/Appends_SamplerStates.hlsli"
#include "../../../Appends/Libraries/SceneInfo/Appends_SceneInfoVSPS.hlsli"

//�ݵ�� ��ǲ = VOutQuad, �ƿ�ǲ = POutQuad
POutQuad main(VOutQuad pin)
{
    POutQuad res;
   
    float4 worldPosition = float4(GetPosition(pin.UV), 1.0f);
    float4 homogeneous = mul(worldPosition, _lightViewProj);
    float depth = homogeneous.z / homogeneous.w;
    res.Output = float4(depth, depth, depth, 1.0f); //Light ���� Depth ���.
    
    //�ϴ���, Light ���� Depth ���.
    return res;
}