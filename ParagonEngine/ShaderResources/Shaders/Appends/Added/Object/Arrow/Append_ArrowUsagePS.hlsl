//Appends PS �ʼ� ����.

//Append Shader�� ���� ���̴� Commons
#include "../../../Libraries/Appends_PSCommon.hlsli"

//������۴� ������ b8���� ����. 

//Texture2D�� ������ t25���� ����.
Texture2D<float4> t2_DiffuseTexture : register(t25);

//�ݵ�� ��ǲ = VOutQuad, �ƿ�ǲ = POutQuad
POutQuad main(VOutQuad pin)
{
    //��� Appends�� Pixel Shader ���� ����, ClipUnfits �Լ��� ����־�� �Ѵ�. ���� Ư�� ����.
    ClipUnfits(pin.UV);
    
    //�������� Shader Code.
    POutQuad res;
    
    res.Output = t2_DiffuseTexture.Sample(defaultTextureSS, GetUV_F2(pin.UV));
        
    return res;
}