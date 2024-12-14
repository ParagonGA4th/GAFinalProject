//Appends VS �ʼ� ����.

//Append Shader�� ���� ���̴� Commons
#include "../../../Libraries/Appends_VSCommon.hlsli"

//�ٸ� ������� �߰��� �� ���� ���̴�.
cbuffer cbAppendsObjectVS : register(b8)
{
    bool toggleNoise; 
};

//�ݵ�� ��ǲ = VinQuad, �ƿ�ǲ = VOutQuad.
VOutQuad main(VinQuad vin)
{   
    VOutQuad vout;
    if (toggleNoise)
    {
        vout.PositionH = float4(vin.Position, 1.0f);
        vout.UV = 1.0f - vin.UV;
    }
    else
    {
        vout.PositionH = float4(vin.Position, 1.0f);
        vout.UV = vin.UV;
    }
    
    return vout;
}

