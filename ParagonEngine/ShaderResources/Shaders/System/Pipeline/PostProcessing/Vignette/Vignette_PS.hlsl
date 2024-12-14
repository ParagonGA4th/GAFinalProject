//�ʿ��� ��Ҹ� ��� ���������� ������ ����.

#include "../../../Libraries/System_QuadLayouts.hlsli"
#include "../../../../Appends/Libraries/SamplerStates/Appends_SamplerStates.hlsli"
#include "../../../../Appends/Libraries/MathFunctions/Appends_MathFunctions.hlsli"
#include "../../../../Appends/Libraries/SceneInfo/Appends_SceneInfoVSPS.hlsli"

Texture2D<float4> QuadRTV : register(t5);

//�ݵ�� ��ǲ = VOutQuad, �ƿ�ǲ = POutQuad
POutQuad main(VOutQuad pin)
{
    POutQuad res;
    
    //��ü�� ToGBuffer���� ������ �ǵ��ۿ� ����. UV ��ȯ�� �ι� ��ĥ �ʿ䰡 ����.
    float3 color = QuadRTV.Sample(fullScreenQuadSS, pin.UV).xyz;
    
    //���� Color Correction ���� ���� ����.    
    //color = CorrectColors(color, pin.UV, 0.1f);
   
    if (gCBuf_IsVignetteOn)
    {
        float2 uv = pin.UV * 2.0 - 1.0;
        float vignette = 1.0 - dot(uv, uv);
        vignette = pow(vignette, 1.5); // ���� ������ ���ؼ� Factor ����.

        // Vignette �� �����Ű��.
        color.rgb *= vignette;
    }
    
    res.Output = float4(color, 1.0f);
    
    return res;
}