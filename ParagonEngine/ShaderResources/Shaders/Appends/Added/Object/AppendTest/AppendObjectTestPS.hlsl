//Appends PS �ʼ� ����.

//Append Shader�� ���� ���̴� Commons
#include "../../../Libraries/Appends_PSCommon.hlsli"
#include "../../../Libraries/MathFunctions/Appends_UVFunctions.hlsli"

//������۴� ������ b8���� ����. 
cbuffer cbAppendsObject : register(b8)
{
    bool isOne;
};

//Texture2D�� ������ t25���� ����.
Texture2D<float4> t2_DiffuseTexture : register(t25);
Texture2D<float4> t2_DiffuseTexture2 : register(t26);

//�ݵ�� ��ǲ = VOutQuad, �ƿ�ǲ = POutQuad
POutQuad main(VOutQuad pin)
{
    //��� Appends�� Pixel Shader ���� ����, ClipUnfits �Լ��� ����־�� �Ѵ�. ���� Ư�� ����.
    ClipUnfits(pin.UV);
    
    //�������� Shader Code.
    POutQuad res;
    
    [flatten]
    if (isOne)
    {
        res.Output = t2_DiffuseTexture.Sample(defaultTextureSS, GetUV_F2(pin.UV));
    }
    else
    {
        float2 tUVF2 = GetUV_F2(pin.UV);
        tUVF2 = TileUV(tUVF2, float2(1, 1), float2(gCBuf_RadianTimeLoop, 0));
        res.Output = t2_DiffuseTexture2.Sample(defaultTextureSS, tUVF2);
    }
    
    //Pseudo-Fog Test.
    //float fogFactor = saturate(1.0f - GetPseudoLinearDepth(pin.UV));
    //float4 fogColor = 1.0f;
    //res.Output = saturate(fogFactor * res.Output + (1.0 - fogFactor) * fogColor);
        
    return res;
}