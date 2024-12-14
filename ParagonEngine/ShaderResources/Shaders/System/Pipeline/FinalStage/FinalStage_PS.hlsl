//SystemQuad���� MainRenderTarget���� �ѷ��ִ� ����. (PS)
#include "../../Libraries/System_QuadLayouts.hlsli"
#include "../../../Appends/Libraries/SamplerStates/Appends_SamplerStates.hlsli"
#include "../../../Appends/Libraries/MathFunctions/Appends_MathFunctions.hlsli"

Texture2D<float4> QuadRTV : register(t5);

//�ݵ�� ��ǲ = VOutQuad, �ƿ�ǲ = POutQuad
POutQuad main(VOutQuad pin)
{
    POutQuad res;
    //��ü�� MainRenderTarget���� ������ �ǵ��ۿ� ����. UV ��ȯ�� �ι� ��ĥ �ʿ䰡 ����.
    float3 x = QuadRTV.Sample(fullScreenQuadSS, pin.UV).xyz;
    
    res.Output = float4(x, 1.0f);
    
    return res;
}